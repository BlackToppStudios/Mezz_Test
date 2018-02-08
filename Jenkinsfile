#!groovy

try {
    stage('Checkout') {
        parallel UbuntuEmscripten: {
            node('UbuntuEmscripten') {
                checkout scm
            }
        }//,
        // Label: {
        //     node('Executorname') {
        //         sh "shell commands here"
        //     }
        // }
    }

    stage('Build') {
        parallel UbuntuEmscripten: {
            node('UbuntuEmscripten') {
                // The first group of variables simulates running source ~/emsdk-portable/emsdk_env.sh as the emscripten
                // portable sdkrequires to work. The next few sets CC and CXX which CMake will use to know to use
                // the emscripten compiler. The last one sets Mezzanine specific variables so packages are found.
                sh """ export PATH=$PATH:/home/cisadmin/emsdk-portable/clang/e1.37.9_64bit                            &&       
                       export PATH=$PATH:/home/cisadmin/emsdk-portable/node/4.1.1_64bit/bin                           &&
                       export PATH=$PATH:/home/cisadmin/emsdk-portable/emscripten/1.37.9                              &&
                       export EMSDK=/home/cisadmin/emsdk-portable                                                     &&
                       export EM_CONFIG=/home/cisadmin/.emscripten                                                    &&
                       export BINARYEN_ROOT=/home/cisadmin/emsdk-portable/clang/e1.37.9_64bit/binaryen                &&
                       export EMSCRIPTEN=/home/cisadmin/emsdk-portable/emscripten/1.37.9                              &&
                       export CC=emcc                                                                                 &&
                       export CXX=em++                                                                                &&
                       export MEZZ_PACKAGE_DIR=/home/cisadmin/Code/                                                   &&
                       mkdir build -p                                                                                 &&
                       cd build                                                                                       &&
                       cmake -G"Ninja" .. -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF                             &&
                       ninja
                """
            }
        }
    }

    stage('Test') {
        parallel UbuntuEmscripten: {
            node('UbuntuEmscripten') {
                sh """ export PATH=$PATH:/home/cisadmin/emsdk-portable/node/4.1.1_64bit/bin                           &&
                       cd build                                                                                       &&
                       node Test_Tester.js all skip-process NoThreads
                """
            }
        }
    }

    stage('SendMail') {
        notifyMail("Success!", "Build of ${env.JOB_NAME} Successful.")
    }
}
catch(buildException) {
    notifyMail("Failure!", "Build of ${env.JOB_NAME} Failed!\nException: ${buildException}")
    throw buildException
}

def notifyMail (def Status, def ExtraInfo) {
    mail to: 'sqeaky@blacktoppstudios.com, makoenergy@blacktoppstudios.com',
         subject: "${Status} - ${env.JOB_NAME}",
         body: "${Status} - ${env.JOB_NAME} - Jenkins Build ${env.BUILD_NUMBER}\n\n" +
               "${ExtraInfo}\n\n" +
               "Check console output at $BUILD_URL to view the results."
}

