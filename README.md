#### What is it?
It is a library to ease the handling application glue in a modular way so dependencies can be left at bare minimum.

If you want to take a look, use doxygen to create documentation, most of the stuff should be pretty well-explained.  If they aren't - leave me an issue.

Note that the API is NOT stable yet.  During 2014 I have been using this for
a lot of different projects, and the requirements of those projects affect which
features are to remain and to be added.  In particular, module_video is currently
going through almost complete rewrite, while the string class is only going to
be expanded if new features are required.

On the other note, while this repository doesn't get standard updates, it doesn't mean that I have stopped working on it, just that the work is done mostly in projects that use this library, then "backported" here.

#### No really, what is it?
It is a framework library.  Or an application/game engine engine.  A newer take on my old Wheel library.  It is still very much a
work in progress, but it is usable enough for me not to focus on it too much at the moment.  Some other projects that I am fighting with
use this, so if there are any decent improvements on the way, they will be added to this repository as well.

#### To get this working
If you are not using x86_64-linux, you need to replace physfs location in CMakeLists.
This is because the current cmakelists links it against static physfs built against
musl (done for debugging purposes), will fix in stable version, or when I get
buildbot running, whichever comes first.

#### What is this about unstable API?
Unfortunatelhy, there are parts that are not up to the task they should be doing, and as such some changes will be made from time to time.  On the other hand, classes such as wheel::String are doing their job very well, and do not
need to be changed.

#### Licence
Copyright (c) 2013-2015 Jari Ronkainen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

