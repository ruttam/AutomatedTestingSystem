// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef TESTCASE_CALLBACK_INTERFACE_HPP
#define TESTCASE_CALLBACK_INTERFACE_HPP

namespace DUTControllerFramework {

    //! @brief An abstract callback object class.
    class TestCaseCallbackInterface {
        public:
            TestCaseCallbackInterface() { }

            virtual ~TestCaseCallbackInterface() { }

            //! @brief A callback function to pass debug data of string type from caller to callee.
            //! @param data Text data of a string type transmited from a caller to a callee.
            virtual void debugData(const std::string& data)=0;

            //! @brief A callback function to pass result data of string type from caller to callee.
            //! @param data Text data of a string type transmited from a caller to a callee.
            virtual void resultData(const std::string& data)=0;
    };

}

#endif
