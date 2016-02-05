#include "Options.h"

StartupOptions get_startup_options(int argc, char** argv)
{
    if (argc == 1)
    {
        return StartupOptions();
    }

    StartupOptions s;
    auto arg = 1U;
    while (arg < argc)
    {
        std::string a = argv[arg];
        if (a == "--width" || a == "-w") { s.width = atoi(argv[++arg]); }
        if (a == "--height" || a == "-h") { s.height = atoi(argv[++arg]); }
        if (a == "--iterations" || a == "-i") { s.iterations = atoi(argv[++arg]); }

        if (a == "--expose" || a == "-e") { s.expose = true; }
        if (a == "--no-expose" || a == "-E") { s.expose = false; }

        if (a == "--develop" || a == "-d") { s.develop = true; }
        if (a == "--no-develop" || a == "-D") { s.develop = false; }

        if (a == "--palette" || a == "-p") { s.palette = argv[++arg]; }
        if (a == "--attractor" || a == "-a") { s.attractor = argv[++arg]; }

        if (a == "--output" || a == "-o") { s.output_filename = argv[++arg]; }
        if (a == "--expose-file") { s.exposed_filename = argv[++arg]; }

        arg++;
    }

    s.Check();
    return s;
}

