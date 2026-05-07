# DL_RobotConfig Calc Dialog Code Notes

## 1. Change Goal

This change refactors the `Calc` button in `DL_RobotConfig` from a direct one-shot backend call into a modeless dialog workflow.

The new workflow supports:

- reading the current robot joint state and TCP pose
- editing a target Cartesian pose manually
- generating random target positions in a user-defined XYZ range
- solving IK for the target pose
- animating the robot from the current joint state to the solved joint state
- running FK/IK consistency analysis for the current pose

The framework layer was not changed for this feature. All implementation stays inside the `DL_RobotConfig` project layer.

## 2. Files Added or Changed

### Newly added

- [DL_RobotCalcDialog.h](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotCalcDialog.h)
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx)

### Changed

- [DLMUI_RobotOps.h](D:/DL_WELD_HOME/include/DL_RobotConfig/DLMUI_RobotOps.h)
- [DLMUI_RobotOps.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DLMUI_RobotOps.cxx)
- [DL_RobotContext.h](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h)
- [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx)
- [DL_RobotConfig.pro](D:/DL_WELD_HOME/make/DL_RobotConfig/DL_RobotConfig.pro)

### Included runtime stabilization

- [view_pan.png](D:/DL_WELD_HOME/src/dlOcctMdiFrame/res/view_pan.png)

This PNG contained an invalid `bKGD` chunk and could trigger startup instability under the old Qt/libpng runtime. The bad chunk was removed without changing the visible icon.

## 3. Entry Call Chain

The new `Calc` feature now follows this path:

1. User clicks the `Calc` toolbar button.
2. [DLMUI_RobotOps::onRobotCalc()](D:/DL_WELD_HOME/src/DL_RobotConfig/DLMUI_RobotOps.cxx:299) checks that the current document has a loaded robot.
3. The function creates or reuses a single modeless [DL_RobotCalcDialog](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:27).
4. The dialog is bound to the active [DocumentTut](D:/DL_WELD_HOME/include/DL_RobotConfig/DocumentTut.h).
5. The dialog talks to [DL_RobotContext](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h) for all robot calculations.

This keeps UI orchestration in `DLMUI_RobotOps`, user interaction in the dialog, and robot math/state in `DL_RobotContext`.

## 4. Why a Separate `DL_RobotCalcDialog.cxx`

The dialog was intentionally not merged into `robotContext.cxx`.

Reasons:

- `DL_RobotContext` already contains XML loading, OCC display sync, RL model handling, STEP splitting, TCP handling, and motion logic.
- The `Calc` workflow is UI-heavy: input fields, random range controls, result text, and button state refresh.
- The dialog is likely to grow in the future, especially when seam trajectory and welding motion features are merged in.
- Keeping the dialog separate makes later migration to another presentation form easier, even if it is still modeless for now.

Merging into `robotContext.cxx` is technically possible, but the code would become much harder to maintain.

## 5. New UI Class: `DL_RobotCalcDialog`

### Role

[DL_RobotCalcDialog](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotCalcDialog.h:18) is the user-facing controller for the new `Calc` workflow.

It is:

- modeless
- reused instead of opening many duplicate windows
- bound to the currently active `DocumentTut`
- automatically invalidated when the document is destroyed

### Key members

- `QPointer<DocumentTut> myDocument`
  - keeps a safe weak reference to the current document
- `QMetaObject::Connection myDocumentDestroyedConnection`
  - disconnects and updates UI when the document is closed
- `QDoubleSpinBox* myTargetPoseEdits[6]`
  - editable target `X/Y/Z/Rx/Ry/Rz`
- `QDoubleSpinBox* myRandomRangeEdits[6]`
  - editable `X min/max`, `Y min/max`, `Z min/max`
- `double mySolvedAngles[DL_ROBOT_JOINT_COUNT]`
  - caches the most recent successful IK result

### Key functions

#### `setDocument(DocumentTut* theDocument)`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:154)

Purpose:

- rebinding the dialog to the active document
- disconnecting the previous destroyed-signal binding
- refreshing current pose information

#### `refreshFromCurrentPose()`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:174)

Purpose:

- read current joints from `DL_RobotContext`
- read current TCP pose from `DL_RobotContext`
- fill the read-only current state area
- initialize target pose to the current pose
- initialize random XYZ range to current pose `+- 200 mm`

#### `randomizeTargetPose()`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:196)

Purpose:

- randomize only `X/Y/Z`
- keep `Rx/Ry/Rz` unchanged
- retry up to 20 times
- filter out unreachable poses by calling backend IK before accepting the random target

This means the dialog does not blindly generate random points. It tries to produce a reachable random target in the current range.

#### `onSolveTargetPose()`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:242)

Purpose:

- solve IK for the current target pose
- show summary text in the dialog
- print the same summary to the terminal
- cache solved joint angles if successful

#### `animateToTargetPose()`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:256)

Purpose:

- first solve the current target pose
- if IK succeeds, call backend animation
- refresh displayed current joints and current pose after the move

#### `analyzeCurrentPose()`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:290)

Purpose:

- analyze the current pose as both FK input and IK target
- show the summary in the dialog
- call `calcRobot()` for detailed terminal output

This is mostly a validation and debugging function.

#### `solveCurrentTarget(DL_RobotCalcReport* theReport)`

Location:
- [DL_RobotCalcDialog.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx:423)

Purpose:

- convert UI values into a `DL_CartesianPose`
- call backend analysis
- cache solved joint angles if successful

This is the dialog's main bridge between UI input and backend calculation.

## 6. New Data Structures in `DL_RobotContext`

### `DL_CartesianPose`

Location:
- [DL_RobotContext.h](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h:34)

Purpose:

- represent one Cartesian TCP pose using project-level units and conventions
- position in `mm`
- orientation in `degree`

Fields:

- `xMm`
- `yMm`
- `zMm`
- `rxDeg`
- `ryDeg`
- `rzDeg`

### `DL_RobotCalcReport`

Location:
- [DL_RobotContext.h](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h:49)

Purpose:

- store the result of one target-pose analysis
- unify dialog display, terminal output, and motion execution

Fields include:

- `success`
- `hasLimitWarning`
- `hasMotionWarning`
- `currentPose`
- `targetPose`
- `currentJointAngles[6]`
- `solvedJointAngles[6]`
- `summary`

## 7. New Backend Functions in `DL_RobotContext`

### `getCurrentTcpPose()`

Location:
- [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:776)

Purpose:

- read the current robot pose from the current joint values
- use the existing FK chain
- return the result in the project-level `mm/deg` format

### `solveTargetPose(const DL_CartesianPose&, ...)`

Location:
- [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:783)

Purpose:

- convert a dialog-level Cartesian pose into an RL transform
- call the existing IK path
- return solved joint angles

This is the main backend entry for dialog-driven target pose solving.

### `analyzeTargetPose(const DL_CartesianPose&)`

Location:
- [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:800)

Purpose:

- gather current pose and current joint state
- solve IK for the target pose
- generate warnings
- build a human-readable summary

Current warning rules:

- any solved joint within `5 deg` of its limit
- any solved joint moving more than `45 deg` from the current pose

This function does not move the robot. It only analyzes.

### `animateToJoints(const double targetAngles[6])`

Location:
- [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:840)

Purpose:

- animate all six joints together
- use cubic interpolation
- call `forwardRobot()` at each step
- update the current viewer during animation

Default timing:

- total duration: `600 ms`
- sample step: `20 ms`

This is joint-space interpolation, not path planning.

## 8. Reused Existing Functions and Logic

This version deliberately reuses the existing FK/IK foundation instead of replacing it.

### Reused IK entry

- [DL_RobotContext::ikSolve()](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:742)

The new dialog does not implement its own inverse kinematics.  
It still goes through the existing `ikSolve()` backend.

That means:

- the old RL numerical IK path is preserved
- the existing seed strategy is preserved
- the behavior remains consistent with the older robot code

### Reused FK entry

- [DL_RobotContext::forwardSolve()](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:760)

The dialog reads the current TCP pose through FK instead of inventing a second pose source.

### Reused display-state sync

- [DL_RobotContext::forwardRobot()](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:531)

Animation still relies on the existing OCC/RL synchronization path.

### Reused pose convention builder

- `buildTcpPoseTransform(...)`
  - existing local helper in [robotContext.cxx](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:221)

The new dialog does not introduce a second pose convention.  
Instead, `transformFromPose()` wraps this existing transform builder.

### Reused diagnostic path

- [DL_RobotContext::calcRobot()](D:/DL_WELD_HOME/src/DL_RobotConfig/robotContext.cxx:1219)

`calcRobot()` was kept, but its role changed:

- before: direct `Calc` button backend
- now: detailed current-pose FK/IK terminal diagnosis

It now reuses `analyzeTargetPose()` rather than duplicating its own solve-result narrative.

## 9. Did This Change the RL Solver Principle

No. The dialog changed the interaction layer, not the RL solving principle.

The actual IK solving still uses the old backend path in `ikSolve()`, which in turn still uses:

- the current displayed joint state as seed
- RL local numerical inverse solving behavior

So this version is a UI/flow refactor plus utility layer expansion, not a solver replacement.

## 10. Random Target Strategy

This first version does not generate a fully random `X/Y/Z/Rx/Ry/Rz`.

Instead:

- `X/Y/Z` are randomized inside user-defined bounds
- `Rx/Ry/Rz` remain unchanged

This was chosen to improve the first-version success rate and reduce invalid targets.

## 11. What This Version Does Not Try to Solve

This initial version intentionally does not add:

- path planning
- collision checking
- self-collision checking
- singularity avoidance
- Cartesian line or arc tracking

It only supports:

- Cartesian target input
- numerical IK
- joint interpolation animation

This keeps the first version small and easy to extend later.

## 12. Suggested Extension Points

For later welding-path work, the cleanest extension points are:

- extend [DL_RobotCalcDialog](D:/DL_WELD_HOME/src/DL_RobotConfig/DL_RobotCalcDialog.cxx) with more controls
- add new report fields to [DL_RobotCalcReport](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h:49)
- add trajectory-level functions in [DL_RobotContext](D:/DL_WELD_HOME/include/DL_RobotConfig/DL_RobotContext.h)

Good future candidates:

- generate straight-line TCP samples
- generate arc TCP samples
- solve each sample point with continuity constraints
- animate welding gun along a sampled path

