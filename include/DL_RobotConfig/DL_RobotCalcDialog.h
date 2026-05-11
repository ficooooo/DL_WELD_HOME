#ifndef DL_ROBOTCALCDIALOG_H
#define DL_ROBOTCALCDIALOG_H

#include "DL_RobotConfig/DL_RobotContext.h"

#include <QDialog>
#include <QMetaObject>
#include <QPointer>

#include <random>

class DocumentTut;
class QDoubleSpinBox;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;

class DL_RobotCalcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DL_RobotCalcDialog(QWidget* theParent = nullptr);
    ~DL_RobotCalcDialog();

    void setDocument(DocumentTut* theDocument);

private slots:
    void refreshFromCurrentPose();
    void randomizeTargetPose();
    void randomizeJointTarget();
    void onSolveTargetPose();
    void animateToTargetPose();
    void analyzeCurrentPose();
    void onDocumentDestroyed();
    void onTargetPoseEdited();

private:
    bool            hasValidDocument(QString* theMessage = nullptr) const;
    DL_RobotContext* robotContext() const;
    DL_CartesianPose poseFromEditors(QDoubleSpinBox* theEditors[6]) const;
    void            setPoseEditors(QDoubleSpinBox* theEditors[6], const DL_CartesianPose& thePose);
    void            setCurrentJointEditors(const double theAngles[DL_ROBOT_JOINT_COUNT]);
    void            setCurrentPoseEditors(const DL_CartesianPose& thePose);
    void            setDefaultRandomRanges(const DL_CartesianPose& thePose);
    void            setResultText(const QString& theText);
    void            applyReport(const DL_RobotCalcReport& theReport);
    void            updateUiEnabledState();
    bool            solveCurrentTarget(DL_RobotCalcReport* theReport);
    void            cacheSolvedAngles(const double theAngles[DL_ROBOT_JOINT_COUNT]);
    void            cacheSampledAngles(const double theAngles[DL_ROBOT_JOINT_COUNT]);
    QString         sampledJointComparisonText(const double theSolvedAngles[DL_ROBOT_JOINT_COUNT]) const;

private:
    QPointer<DocumentTut> myDocument;
    QMetaObject::Connection myDocumentDestroyedConnection;
    QLineEdit*            myCurrentJointEdits[DL_ROBOT_JOINT_COUNT];
    QLineEdit*            myCurrentPoseEdits[6];
    QDoubleSpinBox*       myTargetPoseEdits[6];
    QDoubleSpinBox*       myRandomRangeEdits[6];
    QPlainTextEdit*       myResultEdit;
    QPushButton*          myReadCurrentButton;
    QPushButton*          myRandomButton;
    QPushButton*          myRandomJointButton;
    QPushButton*          mySolveButton;
    QPushButton*          myAnimateButton;
    QPushButton*          myAnalyzeButton;
    bool                  myHasSolvedTarget;
    bool                  myHasSampledJointTarget;
    double                mySolvedAngles[DL_ROBOT_JOINT_COUNT];
    double                mySampledJointAngles[DL_ROBOT_JOINT_COUNT];
    std::mt19937          myRandomEngine;
};

#endif

