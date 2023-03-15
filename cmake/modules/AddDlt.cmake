find_package(PkgConfig)

pkg_check_modules(Dlt REQUIRED IMPORTED_TARGET automotive-dlt)

set(JARVIS_DLT_APPID
    "JRV"
    CACHE STRING "DLT application identifier name" FORCE)
set(JARVIS_DLT_APPID_DESC
    "J.A.R.V.I.S Application"
    CACHE STRING "DLT application identifier description" FORCE)
