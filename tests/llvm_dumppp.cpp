#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <sys/stat.h>

#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCDisassembler/MCRelocationInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Format.h"

using namespace llvm;

typedef struct {
    void *data;
    size_t length;
} buffer;

static buffer parse_hex(int argc, char **argv)
{
    uint8_t *data = (uint8_t*) malloc(argc);
    for (size_t i = 0; i < argc; i++) {
        int x = 0;
        sscanf(argv[i], "%x", &x);
        data[i] = (uint8_t)x;
    }
    buffer buf = { (uint8_t*)data, (size_t)argc };
    return buf;
}

static buffer load_file(const char *filename)
{
    FILE *f;
    struct stat statbuf;
    char *buf;
    size_t nread;

    if ((f = fopen(filename, "r")) == NULL) {
        printf("load_file: open: %s: %s",
            filename, strerror(errno));
        exit(1);
    }
    if (fstat(fileno(f), &statbuf) < 0) {
        printf("load_file: stat: %s: %s",
            filename, strerror(errno));
        exit(1);
    }
    buf = (char*)malloc(statbuf.st_size);
    if ((nread = fread(buf, 1, statbuf.st_size, f)) != statbuf.st_size) {
        printf("load_file: fread: %s: expected %zu got %zu\n",
            filename, (size_t)statbuf.st_size, nread);
        exit(1);
    }
    return (buffer){buf, (size_t)statbuf.st_size};
}

struct LLVMDisassembler
{
    static const int hexcols = 10;

    const Target *tg;
    std::string err;
    MCTargetOptions options;

    std::unique_ptr<MCRegisterInfo> ri;
    std::unique_ptr<MCAsmInfo> ai;
    std::unique_ptr<MCSubtargetInfo> si;
    std::unique_ptr<MCInstrInfo> ii;
    std::unique_ptr<MCContext> cx;
    std::unique_ptr<MCDisassembler> di;
    std::unique_ptr<MCInstPrinter> ip;

    LLVMDisassembler(std::string triple, std::string cpu, std::string features)
    {
        tg = TargetRegistry::lookupTarget(triple, err);
        ri.reset(tg->createMCRegInfo(triple));
        ai.reset(tg->createMCAsmInfo(*ri, triple, options));
        si.reset(tg->createMCSubtargetInfo(triple, cpu, features));
        ii.reset(tg->createMCInstrInfo());
        cx.reset(new MCContext(Triple(triple), ai.get(), ri.get(), si.get()));
        di.reset(tg->createMCDisassembler(*si, *cx));
        ip.reset(tg->createMCInstPrinter(Triple(triple),
            ai->getAssemblerDialect(), *ai, *ii, *ri));
    }

    void format_hex(raw_string_ostream &out, ArrayRef<uint8_t> data,
        size_t offset, size_t sz, int show_off, int show_hex)
    {
        int nbytes = sz < hexcols ? sz : hexcols;
        if (show_off) {
            out << format_hex_no_prefix(offset, 8) << ":";
        }
        if (show_hex) {
            out << '\t';
            out << format_bytes(data.slice(offset, nbytes), {}, hexcols, 1);
            out.indent((hexcols - nbytes) * 3 + 8 - (hexcols * 3) % 8);
        }
    }

    int disasm(size_t offset, ArrayRef<uint8_t> data, int show_off, int show_hex)
    {
        std::string buf;
        raw_string_ostream out(buf);
        MCInst in;
        uint64_t sz;
        while (offset < data.size() &&
               di->getInstruction(in, sz, data.slice(offset), offset, out))
        {
            if (show_off || show_hex) {
                format_hex(out, data, offset, sz, show_off, show_hex);
            }
            ip->printInst(&in, offset, "", *si, out);
            if (sz == 0) break;
            puts(buf.c_str());
            buf.clear();
            if (show_off || show_hex) {
                while (sz > hexcols) {
                    offset += hexcols; sz -= hexcols;
                    format_hex(out, data, offset, sz, show_off, show_hex);
                    printf("%s\n", buf.c_str());
                    buf.clear();
                }
            }
            offset += sz;
        }
        return 0;
    }
};

int main(int argc, char **argv)
{
    buffer buf = { 0 };
    const char *filename = NULL;
    const char *triple = "x86_64";

    int help = 0, show_off = 0, show_hex = 0, i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-h") == 0) {
            help = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            show_off = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            show_hex = 1;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            triple = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else {
            break; /* hexidecimal */
        }
        i++;
    }

    if (!help && !filename && i < argc) {
        buf = parse_hex(argc - i, argv + i);
    }
    else if (help) {
        fprintf(stderr,
            "usage: %s [-o] [-x] [-t <triple>] (-f <file.bin> | <hex> ...)\n",
            argv[0]);
        return -1;
    }
    else if (filename) {
        buf = load_file(filename);
    }

    SmallVector<const char *> Args = { argv[0], "--x86-asm-syntax=intel" };
    llvm::cl::ParseCommandLineOptions(Args.size(), Args.data());

    InitializeAllTargetInfos();
    InitializeAllTargetMCs();
    InitializeAllDisassemblers();

    int ret = 0;
    if (buf.data && buf.length) {
        LLVMDisassembler dis("x86_64", "", "");
        ret = dis.disasm(0, ArrayRef<uint8_t>((uint8_t*)buf.data, buf.length),
            show_off, show_hex);
    }

    return ret;
}
