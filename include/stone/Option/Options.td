// Include the common option parsing interfaces.
include "llvm/Option/OptParser.td"

/////////
// Flags

// The option should be accepted by swift -frontend.
def FrontendOption : OptionFlag;

// The option should be accepted by swift-autolink-extract
def AutolinkExtractOption : OptionFlag;

// The option should be accepted by swift -modulewrap
def ModuleWrapOption : OptionFlag;

// The option should be accepted by swift-indent
def StoneIndentOption : OptionFlag;

// The option should not be accepted by the driver.
def NoDriverOption : OptionFlag;

// Some options should not be available depending on whether this is the
// interactive driver 'swift', or the batch compiler 'swiftc'.
def NoInteractiveOption : OptionFlag;

// The option should not force a full rebuild if added, changed, or removed.
def DoesNotAffectIncrementalBuild : OptionFlag;

// The option's argument is a file-system path that may be affected by the
// current working directory.
def ArgumentIsPath : OptionFlag;

// The option should be written into a .swiftinterface module interface file,
// and read/parsed from there when reconstituting a .swiftmodule from it.
def ModuleInterfaceOption : OptionFlag;

// The option should be written into a .swiftinterface module interface file,
// and read/parsed from there when reconstituting a .swiftmodule from it.
// The option can be safely ignored by the older compiler.
def ModuleInterfaceOptionIgnorable : OptionFlag;

// The option should be written into a .private.swiftinterface module interface file,
// and read/parsed from there when reconstituting a .swiftmodule from it.
// The option can be safely ignored by the older compiler.
def ModuleInterfaceOptionIgnorablePrivate : OptionFlag;

// The option causes the output of a supplementary output, or is the path option
// for a supplementary output. E.g., `-emit-module` and `-emit-module-path`.
def SupplementaryOutput : OptionFlag;

// The option should be accepted by swift-api-extract.
def StoneAPIExtractOption : OptionFlag;

// The option should be accepted by swift-symbolgraph-extract.
def StoneSymbolGraphExtractOption : OptionFlag;

// The option should be accepted by swift-api-digester.
def StoneAPIDigesterOption : OptionFlag;

// The option only functions in the new driver.
def NewDriverOnlyOption : OptionFlag;

// The option's argument is a path to a file that contains a list of files.
def ArgumentIsFileList : OptionFlag;

// The option that is cache invariant. Change this option doesn't affect the
// content of any output file. The candidates are options like: `-o` which only
// affects the location of the output file, or `-emit-module` which defines
// which action is performed.
def CacheInvariant : OptionFlag;


include "FrontendOptions.td"


