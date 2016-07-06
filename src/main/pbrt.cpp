
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// main/pbrt.cpp*
#include "pbrt.h"
#include "api.h"
#include "parser.h"
#include "parallel.h"

// main program
int main(int argc, char *argv[]) {
    Options options;
    std::vector<std::string> filenames;
    // Process command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--nthreads"))
            options.nThreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--outfile"))
            options.imageFile = argv[++i];
        else if (!strcmp(argv[i], "--quick"))
            options.quickRender = true;
        else if (!strcmp(argv[i], "--quiet"))
            options.quiet = true;
        else if (!strcmp(argv[i], "--verbose"))
            options.verbose = true;
        else if (!strcmp(argv[i], "--cat"))
            options.cat = true;
        else if (!strcmp(argv[i], "--toply"))
            options.toPly = true;
        else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-help") ||
                 !strcmp(argv[i], "-h")) {
            printf(R"(usage: pbrt [<options>] <filename.pbrt...>
  --cat                Print a reformatted version of the input file(s) to
                       standard output. Does not render an image.
  --help               Print this help text.
  --nthreads <num>     Use specified number of threads for rendering.
  --outfile <filename> Write the final image to the given filename.
  --quick              Automatically reduce a number of quality settings to
                       render more quickly.
  --quiet              Suppress all text output other than error messages.
  --toply              Print a reformatted version of the input file(s) to
                       standard output and convert all triangle meshes to
                       PLY files. Does not render an image.
  --verbose            Print out more detailed logging information.
)");
            return 0;
        } else
            filenames.push_back(argv[i]);
    }

    // Print welcome banner
    if (!options.quiet && !options.cat && !options.toPly) {
        printf("pbrt version 3 (built %s at %s) [Detected %d cores]\n",
               __DATE__, __TIME__, NumSystemCores());
#ifndef NDEBUG
        printf("*** DEBUG BUILD ***\n");
#endif // !NDEBUG
        printf(
            "Copyright (c)1998-2016 Matt Pharr, Greg Humphreys, and Wenzel "
            "Jakob.\n");
        printf(
            "The source code to pbrt (but *not* the book contents) is covered "
            "by the BSD License.\n");
        printf("See the file LICENSE.txt for the conditions of the license.\n");
        fflush(stdout);
    }
    pbrtInit(options);
    // Process scene description
    if (filenames.size() == 0) {
        // Parse scene from standard input
        ParseFile("-");
    } else {
        // Parse scene from input files
        for (const std::string &f : filenames)
            if (!ParseFile(f))
                Error("Couldn't open scene file \"%s\"", f.c_str());
    }
    pbrtCleanup();
    return 0;
}
