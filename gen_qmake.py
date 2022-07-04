#generate makefile
#author: newsuppy
#date: Sep, 2014
import os
import configparser

platform            = "x"

qt_ver              = "x"
qt_make_win          = "K:/app_lib/qt-5.3.2-vc12/qtbase/bin/qmake"
qt_make_x            = "/opt/qt-5.3.2/qtbase/bin/qmake"
qt_make              = "x"
qt_inc_win          = "K:/app_lib/qt-5.3.2-vc12/qtbase/include"
qt_inc              = "x"
qt_make_x            = "/opt/qt-5.3.2/qtbase/bin/qmake"
qt_inc_x            = "/opt/qt-5.3.2/qtbase/include"
qt_lib_win          = "K:/app_lib/qt-5.3.2-vc12/qtbase/lib"
qt_lib_x            = "/opt/qt-5.3.2/qtbase/lib"
qt_lib              = "x"
qt_path             = "x"

boost_inc_win       = "K:/app_lib/boost_1_56_0_vc12"
boost_inc_x         = "/opt/boost_1_56_0"
boost_inc           = "x"
boost_lib           = "x"

poco_inc_win        = "K:/app_lib/poco-1.4.6p4-all-vc12"
poco_inc_x          = "/opt/poco-1.4.6p4-all"
poco_inc            = "x"
poco_data_inc_win   = "K:/app_lib/poco-1.4.6p4-all-vc12/Data/include"
poco_found_inc_win  = "K:/app_lib/poco-1.4.6p4-all-vc12/Foundation/include"
poco_found_inc_x    = "/opt/poco-1.4.6p4-all/Foundation/include"
poco_found_inc      = "x"
poco_sqlite_inc_win = "K:/app_lib/poco-1.4.6p4-all-vc12/Data/SQLite/include"
poco_crypto_inc_win = "K:/app_lib/poco-1.4.6p4-all-vc12/Crypto/include"
poco_crypto_inc_x   = "/opt/poco-1.4.6p4-all/Crypto/include"
poco_crypto_inc     = "x"
poco_lib_win        = "K:/app_lib/poco-1.4.6p4-all-vc12/lib"
poco_lib_x          = "/opt/poco-1.4.6p4-all/lib/Linux/i686"
poco_lib            = "x"
poco_path           = "x"

project_path        = "x"
project_inc         = "x"

#if (platform == "win"):
#    project_path        = "K:/newsuppy/pydev/gen_qmakefile/nshomeapp"
#    project_path_inc    = "K:/newsuppy/pydev/gen_qmakefile/nshomeapp/include"
#elif (platform == "x"):
#    project_path        = "/opt/nshomeapp"
#    project_path_inc    = "/opt/nshomeapp/include"

makefile_path_win   = project_path + "/Makefile.Release"
makefile_path_x     = project_path + "/Makefile"
makefile_path       = "x"

final_makefile      = project_path + "/makefile.fi"
run_makefile_win    = "nmake -f " + final_makefile
run_makefile_x      = "make -f " + final_makefile
run_makefile        = "x"

is_inc_qtwidgets    = True
is_inc_qtnetwork    = True
is_inc_qttest       = True
is_inc_boost        = True
is_use_boost_lib    = True
is_use_poco         = True
is_inc_poco_found   = True
is_inc_poco_data    = True
is_inc_poco_sqlite  = True
is_inc_poco_crypto  = True

def load_configuration():
    global platform
    global qt_ver
    global project_path
    global project_inc
    global makefile_path
    global qt_path
    global qt_make
    global makefile_path
    global final_makefile
    global run_makefile
    global qt_inc
    global qt_lib
    global boost_inc
    global boost_lib
    global poco_path
    global poco_data_inc
    global poco_found_inc
    global poco_sqlite_inc
    global poco_crypto_inc
    global poco_lib
    config = configparser.ConfigParser()
    config_file = open("gq_env.cfg", "rt")
    config.read_file(config_file)
    platform = config.get("base", "platform")
    print("Platform: ", platform)
    qt_ver = config.get("base", "qt_version")
    print("Qt Version: ", qt_ver)
    project_path = config.get("base", "project_path")
    print("Project Path: ", project_path)
    boost_inc = config.get("base", "boost_path")
    boost_lib = boost_inc + "/stage/lib"
    print("Boost Path: ", boost_inc)
    poco_path = config.get("base", "poco_path")
    print("POCO C++ Path: ", poco_path)
    poco_inc = poco_path
    poco_lib = poco_inc + "/lib"
    print("POCO C++ Lib Path: ", poco_lib)
    project_inc = config.get("base", "project_inc")
    print("Project inc: ", project_inc)

    if (platform == "win"):
        poco_data_inc = poco_inc + "/Data/include"
        poco_found_inc = poco_inc + "/Foundation/include"
        poco_sqlite_inc = poco_inc + "/Data/SQLite/include"
        poco_crypto_inc = poco_inc + "/Crypto/include"
    if (platform == "x"):
        poco_data_inc = poco_inc + "/Data/include"
        poco_found_inc = poco_inc + "/Foundation/include"
        poco_sqlite_inc_win = poco_inc + "/Data/SQLite/include"
        poco_crypto_inc_win = poco_inc + "/Crypto/include"       

    print("Generate Makefile")
    if (platform == "win"):
        makefile_path = project_path + "/Makefile.Release"
    if (platform == "x"):
        makefile_path = project_path + "/Makefile"
    final_makefile = project_path + "/makefile.fi"
    if (platform == "win"):
        run_makefile    = "nmake -f " + final_makefile
    if (platform == "x"):
        run_makefile    = "make -f " + final_makefile
    qt_path = config.get("base", "qt_path")
    if (qt_ver==5):
        qt_make = qt_path + "/qtbase/bin/qmake"
        qt_inc = qt_path + "/qtbase/include"
        qt_lib = qt_path + "/qtbase/lib"
    else:
        qt_make = qt_path + "/bin/qmake"
        qt_inc = qt_path + "/include"
        qt_lib = qt_path + "/lib"
    
   
## end load_configuration

#load configuration
load_configuration()
#init configuration
#if (platform == "win"):
#    qt_make = qt_make_win
#    qt_inc = qt_inc_win
#    qt_lib = qt_lib_win
#    boost_inc = boost_inc_win
#    poco_inc = poco_inc_win
#    poco_found_inc = poco_found_inc_win
#    poco_crypto_inc = poco_crypto_inc_win
#    poco_lib = poco_lib_win
#    run_makefile = run_makefile_win
#    makefile_path = makefile_path_win
#elif (platform == "x"):
#    qt_make = qt_make_x
#    qt_inc = qt_inc_x
#    qt_lib = qt_lib_x
#    boost_inc = boost_inc_x
#    poco_inc = poco_inc_x
#    poco_found_inc = poco_found_inc_x
#    poco_crypto_inc = poco_crypto_inc_x
#    poco_lib = poco_lib_x
#    run_makefile = run_makefile_x
#    makefile_path = makefile_path_x

os.chdir(project_path)

gen_project_file   = qt_make + " -project"
gen_makefile       = qt_make

print("Generate Qt pro file\n")
print(gen_project_file)
os.system(gen_project_file)
print("Generate Makefile\n")
print(gen_makefile)
os.system(gen_makefile)

#read original makefile
print("Reading Original Makefile\n")
orig_makefile = open(makefile_path, 'rt')
orig_makefile_content = orig_makefile.readlines()
final_makefile = open(final_makefile, 'wt')
print("Generating Final Makefile\n")
for line in orig_makefile_content:
    #process makefile
    if (line.startswith('INCPATH')):
        line = line.strip("\n")
        line = line + " -I\"" + project_inc + "\"\n"
        if (is_inc_qtnetwork == True):
            line = line.strip("\n")
            line = line + " -I\"" + qt_inc + "/QtNetwork\"\n"
        if (is_inc_qtwidgets == True):
            line = line.strip("\n")
            line = line + " -I\"" + qt_inc + "/QtWidgets\"\n"
        if (is_inc_boost == True):
            line = line.strip("\n")
            line = line + " -I\"" + boost_inc + "\"\n"
        if (is_inc_poco_found == True):
            line = line.strip("\n")
            line = line + " -I\"" + poco_found_inc + "\"\n"
        if (is_inc_poco_data == True):
            line = line.strip("\n")
            line = line + " -I\"" + poco_data_inc + "\"\n"
        if (is_inc_poco_sqlite == True):
            line = line.strip("\n")
            line = line + " -I\"" + poco_sqlite_inc + "\"\n"
        if (is_inc_poco_crypto == True):
            line = line.strip("\n")
            line = line + " -I\"" + poco_crypto_inc + "\"\n"

    if (line.startswith('LIBS')):
        line = line.strip("\n")
        if (is_inc_qtwidgets == True):
            line = line.strip("\n")    
            if (platform == "win" and qt_ver == "5"):
                line = line + " " + qt_lib + "/Qt5Widgets.lib\n"
            elif (platform == "x" and qt_ver == "5"):
                line = line + " -lQt5Widgets\n"
        if (is_inc_qtnetwork == True):
            line = line.strip("\n")
            if (platform == "win" and qt_ver == "5"):
                line = line + " " + qt_lib + "/Qt5Network.lib\n"
            elif (platform == "x" and qt_ver == "5"):
                line = line + " -lQt5Network\n"
            elif (platform == "win" and qt_ver == "4"):
                line = line + " " + qt_lib + "/QtNetwork4.lib\n"
            elif (platform == "x" and qt_ver == "4"):
                line = line + " -lQtNetwork4\n"
        if (is_inc_qttest == True):
            line = line.strip("\n")
            if (platform == "win" and qt_ver == "5"):
                line = line + " " + qt_lib + "/Qt5Test.lib\n"
            elif (platform == "x" and qt_ver == "5"):
                line = line + " -lQt5Test\n"
            elif (platform == "win" and qt_ver == "4"):
                line = line + " " + qt_lib + "/QtTest4.lib\n"
            elif (platform == "x" and qt_ver == "4"):
                line = line + " -lQtTest4\n"
        if (is_use_poco == True):
            line = line.strip("\n")
            if (platform == "x"):
                line = line + " -L" + poco_lib + "\n"
        if (is_inc_poco_found == True):
            line = line.strip("\n")
            if (platform == "win"):
                line = line + " " + poco_lib + "/PocoFoundation.lib\n"
            elif (platform == "x"):
                line = line + " -lPocoFoundation\n"

    if (line.startswith('LFLAGS')):
        if (is_use_boost_lib == True):
            line = line.strip("\n")
            line = line + " /LIBPATH:\"" + boost_lib + "\"\n"
        if (is_use_poco == True):
            line = line.strip("\n")
            line = line + " /LIBPATH:\"" + poco_lib + "\"\n"
            
    print(line, end='', file=final_makefile)
    
orig_makefile.close()

final_makefile.close()

print("Run Final Makefile\n")
print(run_makefile)
os.system(run_makefile)
    
