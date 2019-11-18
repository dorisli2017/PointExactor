This is a README how to compile and run the LabelExtractor on Linux as well as on Windows 10 with the Windows App "Ubuntu".

RUN (LINUX)
1) install expat and proj4
2) run ./LabelExtractor

RUN (WINDOWS 10)
1) install the Ubuntu app
2) install within the Ubuntu app: expat and proj4
3) install Xming or similar on Windows. This is used as GUI for the Ubuntu app since it has no display itself.
4) set the display and execute the built app "DISPLAY=:0 ./LabelExtractor"

COMPILE (optional, since theLabelExtractor is already compiled and ready to run)
1) install in the ubuntu app: qt5, expat and proj4
2) execute command "qmake"
3) execute command "make" --> will create the "LabelExtractor" file on success

