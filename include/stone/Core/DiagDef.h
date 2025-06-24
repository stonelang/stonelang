/// Define macros
#ifdef DEFINE_DIAG_MACROS

#ifndef DIAG
#define DIAG(ID, CODE, MSG, SIG)
#endif

#ifndef FIXIT
#define FIXIT(ID, MSG, SIG)
#endif

#undef DEFINE_DIAG_MACROS
#endif // DEFINE_DIAG_MACROS

/// Undefine macros
#ifdef UNDEFINE_DIAG_MACROS

#ifndef DIAG_NO_UNDEF

#undef DIAG
#undef FIXIT

#endif // DIAG_NO_UNDEF

#undef UNDEFINE_DIAG_MACROS
#endif // UNDEFINE_DIAG_MACROS
