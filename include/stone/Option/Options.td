// Include the common option parsing interfaces.
include "llvm/Option/OptParser.td"

/////////
// Flags

// The option should be accepted by cram 
def FrontendOption : OptionFlag;

// The option should not be accepted by the build.
def ExcludeFrontendOption : OptionFlag;

// The option should not be accepted by the build.
def DriverOption : OptionFlag;

// The option should not be accepted by the build.
def ExcludeDriverOption : OptionFlag;

// The option should not be accepted by the build.
def DebugOption : OptionFlag;

// The option's argument is a file-system path that may be affected by the
// current working directory.
def ArgumentIsPath : OptionFlag;

// The option should be written into a .stoneinterface module interface file,
// and read/parsed from there when reconstituting a .stonemodule from it.
def ModuleInterfaceOption : OptionFlag;

// The option should be written into a .stoneinterface module interface file,
// and read/parsed from there when reconstituting a .stonemodule from it.
// The option can be safely ignored by the older compiler.
def ModuleInterfaceOptionIgnorable : OptionFlag;

// The option causes the output of a supplementary output, or is the path option
// for a supplementary output. E.g., `-emit-module` and `-emit-module-path`.
def SupplementaryOutput : OptionFlag;

// The option should be accepted by stone-api-extract.
def StoneAPIExtractOption : OptionFlag;

// The option should be accepted by stone-symbolgraph-extract.
def StoneSymbolGraphExtractOption : OptionFlag;

// The option should be accepted by stone-api-digester.
def StoneAPIDigesterOption : OptionFlag;

def ExcludeBatchOption : OptionFlag;

// The option should not force a full rebuild if added, changed, or removed.
def DoesNotAffectIncrementalBuild : OptionFlag;

// The option that is cache invariant. Change this option doesn't affect the
// content of any output file. The candidates are options like: `-o` which only
// affects the location of the output file, or `-emit-module` which defines
// which action is performed.
def CacheInvariant : OptionFlag;

def ModeGroup : OptionGroup<"<mode options>">, HelpText<"MODES">;

class ModeOpt : Group<ModeGroup>;