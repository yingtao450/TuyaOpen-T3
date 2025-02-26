.. _wifi-AT:

**Wi-Fi AT Commands**
=======================

:link_to_translation:`zh_CN:[zh_CN]`


.. important::

    *By default, the Armino supports all the AT commands described in this section*


- :ref:`AT+WLMAC<wifi-mac>` :Get the MAC address of STA identity and SAP identity, you can also configure the MAC address

- :ref:`AT+STASTART<wifi-stastart>` :Connect to the router with the specified SSID as STA

- :ref:`AT+STASTOP<wifi-stastop>` :Disconnect from the router that is currently connected as STA 

- :ref:`AT+WIFISTATUS<wifi-getstatus>` :It is used to view the current mode status of the wifi device or specify parameter information

- :ref:`AT+WIFISCAN<wifi-scan>` :Scan for neighbor AP information

- :ref:`AT+SAPSTART<wifi-sapstart>` :Create SoftAP as SAP

- :ref:`AT+SAPSTOP<wifi-stastop>` :Stop current SoftAP

- :ref:`AT+STAPING<wifi-pingstart>` :PING packets with a specified IP address, a specified duration, and a specified packet size

- :ref:`AT+STAPINGSTOP<wifi-pingstop>` :Stop the PING packet that is being pinged


.. _wifi-mac:

:ref:`AT+WLMAC<wifi-AT>` **:Get the MAC address of STA identity and SAP identity,also configure the MAC address**
--------------------------------------------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+WLMAC = mac_addr

**Params** ::

    No param：Gets the STA/SAP MAC address of the current device
    
    mac_addr(Optional):configure the MAC address of this device

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |     None       |        None         |       
    +---------------+----------------+---------------------+

.. _wifi-stastart:

:ref:`AT+STASTART<wifi-AT>`  **:Connect to the router with the specified SSID as STA**
----------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

     AT+STASTART = ssid, pwd(Optional)

**Params** ::

    SSID：SSID of the AP router to be connected as STA
    
    pwd:  The AP password is ignored if the AP is OPEN

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      10s       |        None         |       
    +---------------+----------------+---------------------+

**AT Event** ::

    EVT:WLAN STA CONNECTED
    EVT:GOT-IP


.. _wifi-stastop:

:ref:`AT+STASTOP<wifi-at>` **:Disconnect from the router that is currently connected as STA**
-----------------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+STASTOP

**Params** ::

    None

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+

**AT Event** ::

    EVT:WLAN STA DISCONNECTED

.. _wifi-getstatus:

:ref:`AT+WIFISTATUS<wifi-at>` **:view the current mode status of the wifi device or specify parameter information**
----------------------------------------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+WIFISTATUS = param(Optional)

**Params** ::

    No param：View the current mode status of the wifi device
    
    RSSI\BSSID\SSID\IP\CHANNEL:Query current status by a specified type.

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+

**AT Event** ::

    EVT:'sta: x, ap: x (1:The corresponding identity has been enabled；0:The corresponding identity has not been enabled)



.. _wifi-scan:

:ref:`AT+WIFISCAN<wifi-at>` **:Scan for neighbor AP information**
----------------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+WIFISCAN = param(Optional)

**Params** ::

    No param：The device scans all surrounding AP information and prints it
    
    param:SSID/BSSID:scan for a specified type

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      4ms       |        None         |       
    +---------------+----------------+---------------------+

**AT Event** ::

    EVT:WLAN STA SCAN_DONE


.. _wifi-sapstart:

:ref:`AT+SAPSTART<wifi-at>` **:Create SoftAP as SAP**
--------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+SAPSTART=param1,param2(Optional)

**Params** ::

    param1：The SSID string you want to create SoftAP
    
    param2 :The password for the SoftAP you want to create, ignore this option if you want to set SAP to OPEN

.. note::

    The password must be longer than 8 bytes. If it is less than 8 bytes, it is processed as OPEN.


**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+

**AT消息** ::

    Print when a STA is connected to this SAP
    EVT:WLAN SAP CONNECTED + MAC Address which belongs to STA


.. _wifi-sapstop:

:ref:`AT+SAPSTOP<wifi-at>` **:Stop current SoftAP**
--------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+SAPSTOP

**Params** ::

    None

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+


.. _wifi-pingstart:

:ref:`AT+STAPING<wifi-at>` **:PING packets with a specified IP address, a specified duration, and a specified packet size**
------------------------------------------------------------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+STAPING=param1,param2,param3

**Params** ::

    param1:Target IP Address
    param2:Packet numbers
    param3:size of each packet

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+

.. _wifi-pingstop:

:ref:`AT+STAPINGSTOP<wifi-at>` **:Stop the PING packet that is being pinged**
-----------------------------------------------------------------------------

**Query command** ::    
    
    None

**Setup command** ::

    AT+STAPINGSTOP

**Params** ::

    None

**Response** ::

    CMDRSP:OK

    CMDRSP:ERROR

**Attributes** :
    +---------------+----------------+---------------------+
    |  Timeout      |  Time-out time |  Time-out callback  | 
    +===============+================+=====================+
    |     None      |      None      |        None         |       
    +---------------+----------------+---------------------+
