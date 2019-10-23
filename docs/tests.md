# Description of tests needed

## Test Bed Platform

The 4x4Platform is the test bed platform targeted by this suite of tests.  

## How do we ensure code coverage of the keymap configurations

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

### Other useful features to test

* KC_TRNS on layers

## Details of all test cases to cover

| Hardware Config        | Layer | Activation Methods | Key       | Implemented             | Results       |
| ---------------------- | ----- | ------------------ | --------- | ----------------------- | ------------- |
| Master                 | main  | PRESS              | Keycode   |  testbench1 - KC_1      | OK            |
|                        | main  | PRESS + OS         | Keycode   |  testbench1 - OS(KC_1)  | BUG: OS active?|
|                        | main  | PRESS + TG         | Keycode   |  testbench1 - TG(KC_1)  | BUG: TG active |
|                        | main  | MT\_TAP            | Keycode   |             |  |
|                        | main  | MT\_HOLD           | Keycode   |             |  |
|                        | main  | DT\_TAP            | Keycode   |             |  |
|                        | main  | DT\_DOUBLTAP       | Keycode   |             |  |
| Master                 | main  | PRESS              | Modifiers |  testbench1 - KC_LSFT       | OK            |
|                        | main  | PRESS + OS         | Modifiers |  testbench1 - OS(KC_LSFT)   | BUG: sent immediately   |
|                        | main  | PRESS + TG         | Modifiers |  testbench1 - TG(KC_LSFT)   | BUG: TG active (keeps repeating immediately)  |
|                        | main  | MT\_TAP            | Modifiers |             |  |
|                        | main  | MT\_HOLD           | Modifiers |             |  |
|                        | main  | DT\_TAP            | Modifiers |             |  |
|                        | main  | DT\_DOUBLTAP       | Modifiers |             |  |
| Master                 | main  | PRESS              | LAYERS    |  testbench1 - LAYER_1  |  |
|                        | main  | PRESS + OS         | LAYERS    |  testbench1 - OS(LAYER_1)  |  |
|                        | main  | PRESS + TG         | LAYERS    |  testbench1 - TG(LAYER_1)  |  |
|                        | main  | MT\_TAP            | LAYERS    |             |  |
|                        | main  | MT\_HOLD           | LAYERS    |             |  |
|                        | main  | DT\_TAP            | LAYERS    |             |  |
|                        | main  | DT\_DOUBLTAP       | LAYERS    |             |  |
| Master                 | main  | PRESS              | Media     |  testbench1 - KC_CALC   | BUG: should send 1, not repeated when held  |
|                        | main  | PRESS + OS         | Media     |  testbench1 - OS(KC_CALC)  | BUG: should send 1, not repeated when held  |
|                        | main  | PRESS + TG         | Media     |  testbench1 - TG(KC_CALC)   | BUG: should send 1, not repeated  |
|                        | main  | MT\_TAP            | Media     |             |  |
|                        | main  | MT\_HOLD           | Media     |             |  |
|                        | main  | DT\_TAP            | Media     |             |  |
|                        | main  | DT\_DOUBLTAP       | Media     |             |  |
| Master                 | main  | PRESS              | Mouse     |  N/A        |  |
|                        | main  | MT\_TAP            | Mouse     |  N/A        |  |
|                        | main  | MT\_HOLD           | Mouse     |  N/A        |  |
|                        | main  | DT\_TAP            | Mouse     |  N/A        |  |
|                        | main  | DT\_DOUBLTAP       | Mouse     |  N/A        |  |
| Master                 | main  | PRESS              | Macro     |  N/A        |  |
|                        | main  | MT\_TAP            | Macro     |  N/A        |  |
|                        | main  | MT\_HOLD           | Macro     |  N/A        |  |
|                        | main  | DT\_TAP            | Macro     |  N/A        |  |
|                        | main  | DT\_DOUBLTAP       | Macro     |  N/A        |  |
| Master                 | main  | PRESS              | Config    |  N/A        |  |
|                        | main  | MT\_TAP            | Config    |  N/A        |  |
|                        | main  | MT\_HOLD           | Config    |  N/A        |  |
|                        | main  | DT\_TAP            | Config    |  N/A        |  |
|                        | main  | DT\_DOUBLTAP       | Config    |  N/A        |  |
| Master                 | layer | PRESS              | Keycode   |  testbench1 - KC_A     | OK |
|                        | layer | PRESS + KC_TRNS    | Keycode   |  testbench1 - KC_1     | OK |
|                        | layer | PRESS + OS         | Keycode   |  testbench1 - OS(KC_A) |  |
|                        | layer | PRESS + TG         | Keycode   |  testbench1 - TG(KC_   |  |
|                        | layer | MT\_TAP            | Keycode   |             |  |
|                        | layer | MT\_HOLD           | Keycode   |             |  |
|                        | layer | DT\_TAP            | Keycode   |             |  |
|                        | layer | DT\_DOUBLTAP       | Keycode   |             |  |
| Master                 | layer | PRESS              | Modifiers |  testbench1 - KC_LCTL  | OK |
|                        | layer | PRESS + KC_TRNS    | Modifiers |  testbench1 - KC_LSFT  | OK |
|                        | layer | PRESS + OS         | Modifiers |  testbench1 - OS(KC_LCTL)  |  |
|                        | layer | PRESS + TG         | Modifiers |  testbench1 - TG(KC_LCTL)  |  |
|                        | layer | MT\_TAP            | Modifiers |             |  |
|                        | layer | MT\_HOLD           | Modifiers |             |  |
|                        | layer | DT\_TAP            | Modifiers |             |  |
|                        | layer | DT\_DOUBLTAP       | Modifiers |             |  |
| Master                 | layer | PRESS              | LAYERS    |             |   |
|                        | layer | PRESS + KC_TRNS    | LAYERS    | testbench1 - LAYER_1   | OK  |
|                        | layer | MT\_TAP            | LAYERS    |             |  |
|                        | layer | MT\_HOLD           | LAYERS    |             |  |
|                        | layer | DT\_TAP            | LAYERS    |             |  |
|                        | layer | DT\_DOUBLTAP       | LAYERS    |             |  |
| Master                 | layer | PRESS              | Media     | testbench1 - KC_WWW  | BUG: should send 1, not repeated when held  |
|                        | layer | PRESS + KC_TRNS    | Media     | testbench1 - KC_CALC  | BUG: should send 1, not repeated when held  |
|                        | layer | PRESS + OS         | Media     | testbench1 - KC_WWW  | BUG: should send 1, not repeated when held  |
|                        | layer | PRESS + TG         | Media     | testbench1 - KC_WWW  | BUG: should send 1, not repeated  |
|                        | layer | MT\_TAP            | Media     |             |  |
|                        | layer | MT\_HOLD           | Media     |             |  |
|                        | layer | DT\_TAP            | Media     |             |  |
|                        | layer | DT\_DOUBLTAP       | Media     |             |  |
| Master                 | layer | PRESS              | Mouse     |  N/A        |  |
|                        | layer | MT\_TAP            | Mouse     |  N/A        |  |
|                        | layer | MT\_HOLD           | Mouse     |  N/A        |  |
|                        | layer | DT\_TAP            | Mouse     |  N/A        |  |
|                        | layer | DT\_DOUBLTAP       | Mouse     |  N/A        |  |
| Master                 | layer | PRESS              | Macro     |  N/A        |  |
|                        | layer | MT\_TAP            | Macro     |  N/A        |  |
|                        | layer | MT\_HOLD           | Macro     |  N/A        |  |
|                        | layer | DT\_TAP            | Macro     |  N/A        |  |
|                        | layer | DT\_DOUBLTAP       | Macro     |  N/A        |  |
| Master                 | layer | PRESS              | Config    |  N/A        |  |
|                        | layer | MT\_TAP            | Config    |  N/A        |  |
|                        | layer | MT\_HOLD           | Config    |  N/A        |  |
|                        | layer | DT\_TAP            | Config    |  N/A        |  |
|                        | layer | DT\_DOUBLTAP       | Config    |  N/A        |  |
