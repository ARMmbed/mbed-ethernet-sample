"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import sys
import uuid
import random
import string
from sys import stdout
from mbed_host_tests import BaseHostTest

class EchoTest(BaseHostTest):

    # Test parameters
    TEST_SERIAL_BAUDRATE = 2*115200
    TEST_LOOP_COUNT = 5
    name = 'echo_mine'

    def test(self, selftest):
        """ This host test will use mbed serial port with
            baudrate 115200 to perform echo test on that port.
        """
        # Custom initialization for echo test
        selftest.mbed.init_serial_params(serial_baud=self.TEST_SERIAL_BAUDRATE)
        selftest.mbed.init_serial()

        # Test function, return True or False to get standard test notification on stdout
        selftest.mbed.flush()
        selftest.notify("HOST: Starting the ECHO test")
        result = True
        for i in range(0, self.TEST_LOOP_COUNT):
            # TEST_STRING = str(uuid.uuid4()) + "\n"
            TEST_STRING = ''.join(random.choice(string.ascii_lowercase + string.digits) for _ in range(30))
            TEST_STRING = TEST_STRING + '\n'
            selftest.mbed.serial_write(TEST_STRING)
            c = selftest.mbed.serial_readline()
            if c is None:
                return selftest.RESULT_IO_SERIAL
            if len(c.strip()) != 30:
                selftest.notify('HOST: "Length mismatch %d"' % len(c.strip()))
            # answer = ''.join([x*2 for x in TEST_STRING])
            answer = TEST_STRING
            if c.strip() != answer.strip():
                selftest.notify('HOST: "%s" != "%s"'% (c, answer))
                result = False
            else:
                sys.stdout.write('.')
                stdout.flush()
        return selftest.RESULT_SUCCESS if result else selftest.RESULT_FAILURE
