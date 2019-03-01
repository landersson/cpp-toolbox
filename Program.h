#pragma once

#include "ProgramOptions.h"

namespace toolbox {

class Program {
public:
    Program();
    virtual ~Program();

    // Run the application.
    // Return value is application's return code.
    virtual int run(int argc, char** argv);

    //! Return the application name
    virtual const char* getName() const = 0;

    //! Return the application name
    virtual const char* getDescription() const = 0;

    //! Return the application's usage examples
    virtual const char* getUsage() { return nullptr; }

    // Get the list of application specific options. Terminated by element with size zero.
    virtual ProgramOption* getOptions() const = 0;

    // Verify that the set of options passed are legal and there is no inconsistencies.
    virtual bool verifyOptions() const { return true; }

    // The main method of the application.
    virtual int main() = 0;

    // overload this to handle command line parsing errors (default = throw up and abort)
    virtual void onCommandLineParseFail(const std::string & message);

    virtual void initialiseOptions();
    virtual void initialise(int argc, char ** argv);
    virtual void terminate();

protected:
    void catchOptionsError(const std::function<void()> & function);
    ProgramOptions _opts;
};

#define PROGRAM_ENTRY_POINT(AppClass) int main(int argc, char ** argv) {  return AppClass().run(argc, argv); }

} // toolbox

