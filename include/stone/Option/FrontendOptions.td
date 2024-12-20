
let Flags = [FrontendOption, NoDriverOption, ArgumentIsFileList] in {

def filelist : Separate<["-"], "filelist">,
  HelpText<"Specify source inputs in a file rather than on the command line">;
def primary_filelist : Separate<["-"], "primary-filelist">,
  HelpText<"Specify primary inputs in a file rather than on the command line">;

}