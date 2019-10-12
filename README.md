# Woterm
custom terminal base on qtermwidget and openssh7.9 and other else tool like zmodem.

## Overview

This project is licensed under the terms of the  [GPLv2](https://www.gnu.org/licenses/gpl-2.0.en.html) or any later version. See the LICENSE file for the full text of the license. 

##How to use?
  1 Build your own sz.exe and rz.exe 
      Download https://github.com/WoTerm/lszrz and open its CMakeList.txt in VS, just build it, copy the exe and .manifest files to somewhere, note down the folder url for post setting.
  2 In Windows10, Add option function "OpenSSH"
  
  3 Build WoTerm.
  4 Modify the ./opt/woterm.ini pointing to right location for ssh, rz and sz.
     ex. ssh :
     ```
     [ssh]
     program=C:\\Windows\\System32\\OpenSSH\\ssh.exe
     ```
  5 Create config file next to bin folder.
  
  Just for memo!
