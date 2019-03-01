
#include "Program.h"

#include "strutil.h"
#include "tokenize.h"
#include <fmt/format.h>

#include "plog/Log.h"
#include "plog/Appenders/RollingFileAppender.h"
#include "plog/Appenders/ConsoleAppender.h"

namespace toolbox {

Program::Program()
{
}

Program::~Program()
{
}

void Program::onCommandLineParseFail(const std::string & message)
{
    exit(1);
}

// call given callback function and catch any boost::program_options related exceptions
void Program::catchOptionsError(const std::function<void()> & function)
{
    try
    {
        function();
    }
    catch (const boost::program_options::error& e)
    {
        fprintf(stderr, "Error parsing command line options: %s\n", e.what());
        this->onCommandLineParseFail(e.what());
    }
}

int Program::run(int argc, char ** argv)
{
    initialiseOptions();

    catchOptionsError(std::bind(&ProgramOptions::parseCommandLine, &_opts, argc, argv));

    if (!_opts["help"].defaulted() || !verifyOptions())
    {
        fprintf(stderr, "%s", getName());
        fprintf(stderr, "  %s", getDescription());
        fprintf(stderr, "\n");

        if (getUsage() != nullptr)
        {
            fprintf(stderr,"Usage:");
            for (std::string usage : strutil::split(getUsage(), ";", false))
            {
                fprintf(stderr,"  %s", usage.c_str());
            }
        }
        fprintf(stderr, "%s\n", _opts.toString().c_str());
        return 127;
    }

    catchOptionsError(std::bind(&ProgramOptions::notify, &_opts));

    try
    {
        _opts.notify();
    }
    catch (const boost::program_options::error& e)
    {
        fprintf(stderr, "Error parsing command line options: %s\n", e.what());
        onCommandLineParseFail(e.what());
    }

    initialise(argc, argv);

    //!!! I've removed the exception catching code.
    //!!! Makes it easier to debug when exception is thrown deep inside.
    //!!! Exceptions seem to work fine now with DNW
    int rv = main();
#if 0
    int return_code = 1;
    try
    {
        runlication();
        return_code = 0;
    }
    catch (const std::exception & le)
    {
        log::error(std::string("Exception: ") + le.what() + "\n\n" + Backtrace::getBacktrace());
        assert(0);
    }
    catch (...)
    {
        log::fatal("Unrecoverable exception %s", Backtrace::getBacktrace().c_str());
    }
#endif
    terminate();

    return rv;
}

void Program::initialiseOptions()
{
    ProgramOption options[] =
    {
        opt::def<bool>("help", "Show Help Message", false),
        //opt::def<bool>("no-header", "Don't show header message", false),
        //opt::def<int>("stack-size", "Override system's default stack size (in mb)", 8),
        //opt::def<std::string>("config", "Program Configuration File ", ""),
        opt::end()
    };

    _opts.addOptions("General Options", options);
    _opts.addOptions("Program Options", getOptions());
    //_options.addOptions("Logging Options", log::getLoggingOptions());
    //_options.addOptions("CPU Options", parallel::getOptions());
}


std::string standardHeader(int argc, char **argv)
{
    std::string header;

    //header += SUT::stringf("--- START OF LOG HEADER --------------------------------\n");
    //header += SUT::stringf("program : %s\n", SysInfo::exePath().c_str());
    //header += SUT::stringf("pid     : %d\n", SysInfo::pid());
    //header += SUT::stringf("host    : %s\n", SysInfo::hostname().c_str());
    //header += SUT::stringf("user    : %s\n", SysInfo::user().c_str());
    //header += SUT::stringf("time    : %s\n", SUT::local_time_string().c_str());
    //header += SUT::stringf("vcpus   : %d\n", SysInfo::cpuCount());
    //header += SUT::stringf("current : %s\n", SysInfo::cpuBrand().c_str());
    //header += SUT::stringf(" vendor : %s\n", SysInfo::cpuVendor().c_str());
    //header += SUT::stringf(" cores  : %d/%d\n", SysInfo::cpuPhysicalCores(), SysInfo::cpuLogicalCores());
    //header += SUT::stringf(" feats  : %s\n", SysInfo::cpuFeaturesHex().c_str());
    //header += SUT::stringf("cwd     : %s\n", SysInfo::currentDir().c_str());
    //header += SUT::stringf("version : %s\n", LUCY_VERSION_STRING);
    //header += SUT::stringf("built @ : %s, %s\n", __DATE__, __TIME__);
    //if (argv != nullptr)
    //{
        //header += SUT::stringf("--- Command Line ---------------------------------------\n");
        //header += SUT::stringf("%s\n", SUT::concat_cstrings(argc, argv).c_str());
    //}
    //header += SUT::stringf("--------------------------------------------------------\n");
    return header;
}


void Program::initialise(int argc, char ** argv)
{
    //log::initialise(_options);

    //if (_options["no-header"].defaulted())
    //{
        //fprintf(stderr,standardHeader(argc, argv));
        //log::flush();
    //}

    //if (!_options["config"].defaulted())
    //{
        //_options.parseConfigFile(_options["config"].as<std::string>(), getName());
        //log::initialise(_options);
    //}

    //if (!_options["stack-size"].defaulted())
        //SysInfo::setStackSize(_options["stack-size"].as<int>());

    //SysInfo::disableCoreDumping();
}


void Program::terminate()
{
    //dump_resource_usage();
    //log::flush();
}

} // toolbox
