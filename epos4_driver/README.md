# EPOS4 Driver (Modified CST PDO)

## EtherCAT module CST mapping – Modified values for TxPDO

```diff
1st 0x6041-00 Statusword

2nd 0x6064-00 Position actual value

! 3rd 0x30D3-01 Velocity actual value averaged

! 4th 0x30D2-01 Torque actual value averaged

5th 0x6061-00 Modes of operation display

6th 0x60FD-00 Digital inputs

+ 7th 0x3160-01 Analog input 1 voltage

+ 8th 0x603F-00 Error code
```

## EtherCAT module CST mapping – Modified values for RxPDO

```diff

1st 0x6040-00 Controlword

2nd 0x6071-00 Target torque

3rd 0x60B2-00 Torque offset

4th 0x6060-00 Modes of operation

5th 0x60FE-01 Digital outputs

+ 6th 0x3001-02 Output current limit
```

Author: 
stefano.dallagasperina@polimi.it

Reference:
EPOS4 Firmware Specification
CCMC | 2019-11 | rel8758

