# Description of tests needed

## Test Bed Platform

The 4x4Platform is the test bed platform targeted by this suite of tests.  

## How do we ensure code coverage of the keymap configurations?
We need to cover a number of permutations for all the options in the following categories:

### Hardware configurations

* Master (standalone keyboard - no comms with other boards)
* Left (Hub for split boards - receives status from spokes)
* Right (Spoke for split boards - sends status to Hub)

### Activation Methods

* PRESS
* MT\_TAP  
* MT\_HOLD
* DT\_TAP
* DT\_DOUBLTAP

### Activation duration

* TG() toggle
* OS() one shot

### Key Types and Code Pages

* KeyCodes
* Modifiers
* LAYERS
* Media
* Mouse
* Config
* Macro
