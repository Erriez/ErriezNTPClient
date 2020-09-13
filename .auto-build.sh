#/bin/bash

# Automatic build script
#
# To run locally, execute:
# sudo apt install doxygen graphviz texlive-latex-base texlive-latex-recommended texlive-pictures texlive-latex-extra

# Exit immediately if a command exits with a non-zero status.
set -e

echo "Starting auto-build script..."


function autobuild()
{
    # Set environment variables
    BOARDS_AVR="--board uno"
    BOARDS_ESP8266="--board d1_mini"
    BOARTS_ESP32="--board lolin_d32"

    echo "Installing library dependencies"
    platformio lib --global install "arduino-libraries/Ethernet"

    echo "Building examples..."
    platformio ci --lib="." ${BOARDS_AVR} examples/AVR/ErriezAVRNTPClient/ErriezAVRNTPClient.ino
    platformio ci --lib="." ${BOARDS_ESP8266} -O lib_ignore=Ethernet examples/ESP8266/ErriezESP8266NTPClient/ErriezESP8266NTPClient.ino
    platformio ci --lib="." ${BOARTS_ESP32} examples/ESP32/ErriezESP32NTPClient/ErriezESP32NTPClient.ino
}

function generate_doxygen()
{
    echo "Generate Doxygen HTML..."

    DOXYGEN_PDF="ErriezNTPClient.pdf"

    # Cleanup
    rm -rf html latex

    # Generate Doxygen HTML and Latex
    doxygen Doxyfile

    # Allow filenames starting with an underscore    
    echo "" > html/.nojekyll

    # Generate PDF when script is not running on Travis-CI
    if [[ -z ${TRAVIS_BUILD_DIR} ]]; then
        # Generate Doxygen PDF
        make -C latex

        # Copy PDF to root directory
        cp latex/refman.pdf ./${DOXYGEN_PDF}
    fi
}

autobuild
generate_doxygen
