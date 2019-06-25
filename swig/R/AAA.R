# first running to load DLL if windows

.onLoad <-function(lib, pkg) {
    if (.Platform$OS.type == "windows") {
        arch_type <- .Platform$r_arch
        print(arch_type)
        dll_path <- system.file("libs/x64/libgeoda.dll", package="libgeoda")
        if (arch_type == "i386") {
            dll_path <- system.file("libs/i386/libgeoda.dll", package="libgeoda")
        } 
        dyn.load(dll_path)
        cacheMetaData(1)
    } else {
        # unix
        dyn.load("libgeoda.so")
        cacheMetaData(1)
    }
}