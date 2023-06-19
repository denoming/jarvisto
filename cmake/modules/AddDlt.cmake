find_package(PkgConfig)

pkg_check_modules(Dlt REQUIRED IMPORTED_TARGET automotive-dlt)

set(DLT_APPID
    "JRV"
    CACHE STRING "DLT application identifier name" FORCE)
set(DLT_APPID_DESC
    "Application"
    CACHE STRING "DLT application identifier description" FORCE)
