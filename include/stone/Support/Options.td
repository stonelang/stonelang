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
def NotForDriverOption : OptionFlag;

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


def InternalGroup : OptionGroup<"<stone internal options>">;
def InternalDebugGroup :
OptionGroup<"<stone debug/development internal options>">,
Group<InternalGroup>, HelpText<"DEBUG/DEVELOPMENT OPTIONS">;

class InternalDebugOpt : Group<InternalDebugGroup>,
Flags<[HelpHidden]>;

// MODES 
def PrintHelp : Flag<["-", "--"], "help">, 
 	Flags<[FrontendOption, DriverOption]>,
  	HelpText<"Display available options">, ModeOpt;
def h : Flag<["-"], "h">, Alias<PrintHelp>;

def PrintHelpHidden : Flag<["-", "--"], "help-hidden">,
  	Flags<[FrontendOption,  DriverOption]>,
  	HelpText<"Display available options, including hidden options">;

def PrintVersion : Flag<["-", "--"], "version">,
  	Flags<[FrontendOption, DriverOption]>,
  	HelpText<"Print version information and exit">, ModeOpt;

def PrintFeature : Flag<["-", "--"], "feature">,
  	Flags<[FrontendOption, DriverOption]>,
  	HelpText<"Display features">, ModeOpt;

def Parse: Flag<["-"], "parse">,
  	HelpText<"Parse input file(s)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;

def ResolveImports: Flag<["-"], "resolve-imports">,
	HelpText<"Parse input file(s)">, ModeOpt,
	Flags<[FrontendOption, DriverOption]>;

def EmitParse : Flag<["-"], "emit-parse">,
 	HelpText<"Parse and emit the ast">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;

def TypeCheck : Flag<["-"], "type-check">,
 	HelpText<"Parse and type-check input file(s)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;

def EmitAST : Flag<["-"], "emit-ast">,
	HelpText<"Parse, type-check, and emit ast">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;

def EmitIR : Flag<["-"], "emit-ir-before">,
  	HelpText<"Emit LLVM IR file(s)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;
 
def EmitModule : Flag<["-"], "emit-module">,
  HelpText<"Emit module file(s)">, ModeOpt,
  Flags<[FrontendOption, DriverOption]>;

def EmitModulePath : Separate<["-"], "emit-module-path">,
  	Flags<[FrontendOption, ArgumentIsPath, SupplementaryOutput, CacheInvariant]>,
  	HelpText<"Emit an importable module to <path>">,
  	MetaVarName<"<path>">;

def EmitModulePathEQ : Joined<["-"], "emit-module-path=">,
  	Flags<[FrontendOption, ArgumentIsPath, SupplementaryOutput, CacheInvariant]>,
  	Alias<EmitModulePath>;

def EmitBC : Flag<["-"], "emit-bc">,
  	HelpText<"Emit LLVM BC file(s)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;

def EmitObject : Flag<["-"], "emit-object">,
  	HelpText<"Emit object file(s) (-c)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;
def c : Flag<["-"], "c">, Alias<EmitObject>,
  	Flags<[FrontendOption, DriverOption]>, ModeOpt;

def EmitAssembly : Flag<["-"], "emit-assembly">,
  	HelpText<"Emit assembly file(s) (-s)">, ModeOpt,
  	Flags<[FrontendOption, DriverOption]>;
def s: Flag<["-"], "S">, Alias<EmitAssembly>,
	Flags<[FrontendOption, DriverOption]>, ModeOpt;

def EmitStaticLibrary : Flag<["-"], "emit-static-library">,
  	HelpText<"Emit a linked library">, ModeOpt,
  	Flags<[DriverOption]>;

def EmitDynamicLibrary : Flag<["-"], "emit-dynamic-library">,
  	HelpText<"Emit a linked library">, ModeOpt,
  	Flags<[DriverOption]>;

def EmitExecutable : Flag<["-"], "emit-executable">,
  	HelpText<"Emit a linked library">, ModeOpt,
  	Flags<[DriverOption]>;


// TARGET
def Target : Separate<["-"], "target">,
  Flags<[FrontendOption, DriverOption]>,
  HelpText<"Generate code for the given target">;

def TargetVariant : Separate<["-"], "target-variant">,
  Flags<[FrontendOption, DriverOption]>,
  HelpText<"Generate 'zippered' code for macCatalyst that can run on the specified"
           " variant target triple in addition to the main -target triple">;

def TargetCPU : Separate<["-"], "target-cpu">, Flags<[FrontendOption, DriverOption]>,
  HelpText<"Generate code for a particular CPU variant">;


// DIAGNOSTICS 
def NoColorDiagnostics : Flag<["-"], "no-color-diagnostics">,
  Flags<[FrontendOption, DoesNotAffectIncrementalBuild]>,
  HelpText<"Print diagnostics in color">;


// LTO
def LTO : Joined<["-"], "lto=">,
  Flags<[FrontendOption]>,
  HelpText<"Specify the LTO type to either 'llvm-thin' or 'llvm-full'">;

def LTOLibrary : Separate<["-"], "lto-library">,
  Flags<[FrontendOption, ArgumentIsPath]>,
  HelpText<"Perform LTO with <lto-library>">, MetaVarName<"<lto-library>">;


