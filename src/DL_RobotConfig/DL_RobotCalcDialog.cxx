#include "DL_RobotConfig/DL_RobotCalcDialog.h"

#include "DL_RobotConfig/DocumentTut.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <rl/math/Unit.h>

namespace
{
QString poseValueText(const double theValue)
{
    return QString::number(theValue, 'f', 3);
}
}

DL_RobotCalcDialog::DL_RobotCalcDialog(QWidget* theParent)
: QDialog(theParent),
  myResultEdit(nullptr),
  myReadCurrentButton(nullptr),
  myRandomButton(nullptr),
  myRandomJointButton(nullptr),
  mySolveButton(nullptr),
  myAnimateButton(nullptr),
  myAnalyzeButton(nullptr),
  myHasSolvedTarget(false),
  myHasSampledJointTarget(false),
  myRandomEngine(std::random_device()())
{
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
    {
        myCurrentJointEdits[i] = nullptr;
        mySolvedAngles[i] = 0.0;
        mySampledJointAngles[i] = 0.0;
    }

    for (int i = 0; i < 6; ++i)
    {
        myCurrentPoseEdits[i] = nullptr;
        myTargetPoseEdits[i] = nullptr;
        myRandomRangeEdits[i] = nullptr;
    }

    setAttribute(Qt::WA_DeleteOnClose, true);
    setModal(false);
    setWindowTitle(QObject::tr("Robot Calc"));
    resize(780, 620);

    QVBoxLayout* aMainLayout = new QVBoxLayout(this);

    QGroupBox* aCurrentGroup = new QGroupBox(QObject::tr("Current State"));
    QGridLayout* aCurrentLayout = new QGridLayout(aCurrentGroup);
    QStringList aJointLabels = QStringList() << "J1(deg)" << "J2(deg)" << "J3(deg)"
                                             << "J4(deg)" << "J5(deg)" << "J6(deg)";
    QStringList aPoseLabels = QStringList() << "X(mm)" << "Y(mm)" << "Z(mm)"
                                            << "Rx(deg)" << "Ry(deg)" << "Rz(deg)";
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
    {
        QLabel* aJointLabel = new QLabel(aJointLabels[i]);
        myCurrentJointEdits[i] = new QLineEdit("0.000");
        myCurrentJointEdits[i]->setReadOnly(true);
        myCurrentJointEdits[i]->setAlignment(Qt::AlignCenter);
        aCurrentLayout->addWidget(aJointLabel, i, 0);
        aCurrentLayout->addWidget(myCurrentJointEdits[i], i, 1);

        QLabel* aPoseLabel = new QLabel(aPoseLabels[i]);
        myCurrentPoseEdits[i] = new QLineEdit("0.000");
        myCurrentPoseEdits[i]->setReadOnly(true);
        myCurrentPoseEdits[i]->setAlignment(Qt::AlignCenter);
        aCurrentLayout->addWidget(aPoseLabel, i, 2);
        aCurrentLayout->addWidget(myCurrentPoseEdits[i], i, 3);
    }
    aMainLayout->addWidget(aCurrentGroup);

    QGroupBox* aTargetGroup = new QGroupBox(QObject::tr("Target Pose"));
    QGridLayout* aTargetLayout = new QGridLayout(aTargetGroup);
    for (int i = 0; i < 6; ++i)
    {
        QLabel* aLabel = new QLabel(aPoseLabels[i]);
        myTargetPoseEdits[i] = new QDoubleSpinBox();
        myTargetPoseEdits[i]->setDecimals(3);
        myTargetPoseEdits[i]->setAlignment(Qt::AlignCenter);
        myTargetPoseEdits[i]->setRange(i < 3 ? -100000.0 : -360.0, i < 3 ? 100000.0 : 360.0);
        myTargetPoseEdits[i]->setSingleStep(i < 3 ? 10.0 : 1.0);
        aTargetLayout->addWidget(aLabel, i / 3, (i % 3) * 2);
        aTargetLayout->addWidget(myTargetPoseEdits[i], i / 3, (i % 3) * 2 + 1);
        connect(myTargetPoseEdits[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DL_RobotCalcDialog::onTargetPoseEdited);
    }
    aMainLayout->addWidget(aTargetGroup);

    QGroupBox* aRangeGroup = new QGroupBox(QObject::tr("Random Range"));
    QGridLayout* aRangeLayout = new QGridLayout(aRangeGroup);
    QStringList aRangeLabels = QStringList() << QObject::tr("X min(mm)") << QObject::tr("X max(mm)")
                                             << QObject::tr("Y min(mm)") << QObject::tr("Y max(mm)")
                                             << QObject::tr("Z min(mm)") << QObject::tr("Z max(mm)");
    for (int i = 0; i < 6; ++i)
    {
        QLabel* aLabel = new QLabel(aRangeLabels[i]);
        myRandomRangeEdits[i] = new QDoubleSpinBox();
        myRandomRangeEdits[i]->setDecimals(3);
        myRandomRangeEdits[i]->setAlignment(Qt::AlignCenter);
        myRandomRangeEdits[i]->setRange(-100000.0, 100000.0);
        myRandomRangeEdits[i]->setSingleStep(10.0);
        aRangeLayout->addWidget(aLabel, i / 2, (i % 2) * 2);
        aRangeLayout->addWidget(myRandomRangeEdits[i], i / 2, (i % 2) * 2 + 1);
    }
    aMainLayout->addWidget(aRangeGroup);

    QHBoxLayout* aButtonLayout = new QHBoxLayout();
    myReadCurrentButton = new QPushButton(QObject::tr("Read Current Pose"));
    myRandomButton = new QPushButton(QObject::tr("Random XYZ"));
    myRandomJointButton = new QPushButton(QObject::tr("Random Joint Sample"));
    mySolveButton = new QPushButton(QObject::tr("Solve IK"));
    myAnimateButton = new QPushButton(QObject::tr("Animate To Target"));
    myAnalyzeButton = new QPushButton(QObject::tr("Analyze Current Pose"));
    QPushButton* aCloseButton = new QPushButton(QObject::tr("Close"));
    aButtonLayout->addWidget(myReadCurrentButton);
    aButtonLayout->addWidget(myRandomButton);
    aButtonLayout->addWidget(myRandomJointButton);
    aButtonLayout->addWidget(mySolveButton);
    aButtonLayout->addWidget(myAnimateButton);
    aButtonLayout->addWidget(myAnalyzeButton);
    aButtonLayout->addStretch();
    aButtonLayout->addWidget(aCloseButton);
    aMainLayout->addLayout(aButtonLayout);

    QGroupBox* aResultGroup = new QGroupBox(QObject::tr("Result Summary"));
    QVBoxLayout* aResultLayout = new QVBoxLayout(aResultGroup);
    myResultEdit = new QPlainTextEdit();
    myResultEdit->setReadOnly(true);
    myResultEdit->setMinimumHeight(180);
    aResultLayout->addWidget(myResultEdit);
    aMainLayout->addWidget(aResultGroup);

    connect(myReadCurrentButton, SIGNAL(clicked()), this, SLOT(refreshFromCurrentPose()));
    connect(myRandomButton, SIGNAL(clicked()), this, SLOT(randomizeTargetPose()));
    connect(myRandomJointButton, SIGNAL(clicked()), this, SLOT(randomizeJointTarget()));
    connect(mySolveButton, SIGNAL(clicked()), this, SLOT(onSolveTargetPose()));
    connect(myAnimateButton, SIGNAL(clicked()), this, SLOT(animateToTargetPose()));
    connect(myAnalyzeButton, SIGNAL(clicked()), this, SLOT(analyzeCurrentPose()));
    connect(aCloseButton, SIGNAL(clicked()), this, SLOT(close()));

    setResultText(QObject::tr("Please open a loaded robot document first."));
    updateUiEnabledState();
}

DL_RobotCalcDialog::~DL_RobotCalcDialog()
{
}

void DL_RobotCalcDialog::setDocument(DocumentTut* theDocument)
{
    if (myDocumentDestroyedConnection)
    {
        QObject::disconnect(myDocumentDestroyedConnection);
        myDocumentDestroyedConnection = QMetaObject::Connection();
    }

    myDocument = theDocument;
    myHasSolvedTarget = false;
    myHasSampledJointTarget = false;
    if (myDocument)
        myDocumentDestroyedConnection = connect(myDocument.data(), SIGNAL(destroyed(QObject*)), this, SLOT(onDocumentDestroyed()));

    updateUiEnabledState();
    if (myDocument)
        refreshFromCurrentPose();
    else
        setResultText(QObject::tr("Current document is invalid. Please reopen a robot document."));
}

void DL_RobotCalcDialog::refreshFromCurrentPose()
{
    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        setResultText(aMessage);
        updateUiEnabledState();
        return;
    }

    DL_RobotContext* aRobot = robotContext();
    const double* aAngles = aRobot->getPositions();
    DL_CartesianPose aPose = aRobot->getCurrentTcpPose();
    setCurrentJointEditors(aAngles);
    setCurrentPoseEditors(aPose);
    setPoseEditors(myTargetPoseEdits, aPose);
    setDefaultRandomRanges(aPose);
    myHasSolvedTarget = false;
    myHasSampledJointTarget = false;
    setResultText(QObject::tr("Current pose loaded."));
    updateUiEnabledState();
}

void DL_RobotCalcDialog::randomizeTargetPose()
{
    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        setResultText(aMessage);
        return;
    }

    const double aXMin = myRandomRangeEdits[0]->value();
    const double aXMax = myRandomRangeEdits[1]->value();
    const double aYMin = myRandomRangeEdits[2]->value();
    const double aYMax = myRandomRangeEdits[3]->value();
    const double aZMin = myRandomRangeEdits[4]->value();
    const double aZMax = myRandomRangeEdits[5]->value();
    if (aXMin > aXMax || aYMin > aYMax || aZMin > aZMax)
    {
        setResultText(QObject::tr("Invalid random range: each min must be less than or equal to max."));
        return;
    }

    DL_CartesianPose aPose = poseFromEditors(myTargetPoseEdits);
    std::uniform_real_distribution<double> aRandX(aXMin, aXMax);
    std::uniform_real_distribution<double> aRandY(aYMin, aYMax);
    std::uniform_real_distribution<double> aRandZ(aZMin, aZMax);
    DL_RobotContext* aRobot = robotContext();
    QString aFailureMessage = QObject::tr("No reachable pose found inside the current random range.");
    double aSolvedAngles[DL_ROBOT_JOINT_COUNT] = {0.0};
    for (int aAttempt = 0; aAttempt < 20; ++aAttempt)
    {
        DL_CartesianPose aCandidate = aPose;
        aCandidate.xMm = aRandX(myRandomEngine);
        aCandidate.yMm = aRandY(myRandomEngine);
        aCandidate.zMm = aRandZ(myRandomEngine);
        if (aRobot->solveTargetPose(aCandidate, aSolvedAngles, &aFailureMessage))
        {
            setPoseEditors(myTargetPoseEdits, aCandidate);
            myHasSolvedTarget = false;
            setResultText(QObject::tr("A reachable random target was generated. Run IK or animate to target."));
            return;
        }
    }

    setResultText(aFailureMessage);
}

void DL_RobotCalcDialog::randomizeJointTarget()
{
    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        setResultText(aMessage);
        return;
    }

    DL_RobotContext* aRobot = robotContext();
    DL_CartesianPose aPose;
    double aSampledAngles[DL_ROBOT_JOINT_COUNT] = {0.0};
    double aSolvedAngles[DL_ROBOT_JOINT_COUNT] = {0.0};
    if (!aRobot->sampleRandomJointTarget(aPose, aSampledAngles, aSolvedAngles, &aMessage))
    {
        myHasSolvedTarget = false;
        myHasSampledJointTarget = false;
        setResultText(aMessage);
        updateUiEnabledState();
        return;
    }

    setPoseEditors(myTargetPoseEdits, aPose);
    cacheSampledAngles(aSampledAngles);
    cacheSolvedAngles(aSolvedAngles);

    QStringList aLines;
    aLines << aMessage;
    aLines << QObject::tr("Target pose was generated from FK of a random joint sample.");
    aLines << sampledJointComparisonText(aSolvedAngles);
    aLines << QObject::tr("You can run Solve IK again or animate directly.");
    setResultText(aLines.join("\n"));
    updateUiEnabledState();
}

void DL_RobotCalcDialog::onSolveTargetPose()
{
    DL_RobotCalcReport aReport;
    if (!solveCurrentTarget(&aReport))
    {
        applyReport(aReport);
        if (!aReport.summary.isEmpty()) std::printf("%s\n", aReport.summary.toLocal8Bit().constData());
        return;
    }

    applyReport(aReport);
    if (!aReport.summary.isEmpty()) std::printf("%s\n", aReport.summary.toLocal8Bit().constData());
}

void DL_RobotCalcDialog::animateToTargetPose()
{
    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        setResultText(aMessage);
        return;
    }

    DL_RobotCalcReport aReport;
    if (!solveCurrentTarget(&aReport))
    {
        applyReport(aReport);
        if (!aReport.summary.isEmpty()) std::printf("%s\n", aReport.summary.toLocal8Bit().constData());
        return;
    }

    applyReport(aReport);
    if (!aReport.summary.isEmpty()) std::printf("%s\n", aReport.summary.toLocal8Bit().constData());

    DL_RobotContext* aRobot = robotContext();
    if (!aRobot->animateToJoints(mySolvedAngles))
    {
        setResultText(myResultEdit->toPlainText() + "\n\n" + QObject::tr("Animation failed."));
        return;
    }

    const double* aAngles = aRobot->getPositions();
    DL_CartesianPose aPose = aRobot->getCurrentTcpPose();
    setCurrentJointEditors(aAngles);
    setCurrentPoseEditors(aPose);
    setResultText(myResultEdit->toPlainText() + "\n\n" + QObject::tr("Animation completed."));
}

void DL_RobotCalcDialog::analyzeCurrentPose()
{
    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        setResultText(aMessage);
        return;
    }

    DL_RobotContext* aRobot = robotContext();
    const double* aAngles = aRobot->getPositions();
    DL_CartesianPose aPose = aRobot->getCurrentTcpPose();
    setCurrentJointEditors(aAngles);
    setCurrentPoseEditors(aPose);

    DL_RobotCalcReport aReport = aRobot->analyzeTargetPose(aPose);
    setResultText(aReport.summary);
    myHasSolvedTarget = false;
    updateUiEnabledState();
    aRobot->calcRobot();
}

void DL_RobotCalcDialog::onDocumentDestroyed()
{
    myDocument = nullptr;
    myHasSolvedTarget = false;
    myHasSampledJointTarget = false;
    setResultText(QObject::tr("Current document was closed. Please reopen a robot document."));
    updateUiEnabledState();
}

void DL_RobotCalcDialog::onTargetPoseEdited()
{
    myHasSolvedTarget = false;
    myHasSampledJointTarget = false;
    updateUiEnabledState();
}

bool DL_RobotCalcDialog::hasValidDocument(QString* theMessage) const
{
    if (myDocument.isNull())
    {
        if (theMessage) *theMessage = QObject::tr("No active robot document is available.");
        return false;
    }

    if (myDocument->lastOpenResult() <= 0)
    {
        if (theMessage) *theMessage = QObject::tr("The current document has no loaded robot.");
        return false;
    }

    DL_RobotContext* aRobot = myDocument->getRobot();
    if (nullptr == aRobot || !aRobot->isLoaded())
    {
        if (theMessage) *theMessage = QObject::tr("Robot context is unavailable.");
        return false;
    }

    return true;
}

DL_RobotContext* DL_RobotCalcDialog::robotContext() const
{
    return myDocument.isNull() ? nullptr : myDocument->getRobot();
}

DL_CartesianPose DL_RobotCalcDialog::poseFromEditors(QDoubleSpinBox* theEditors[6]) const
{
    DL_CartesianPose aPose;
    aPose.xMm = theEditors[0]->value();
    aPose.yMm = theEditors[1]->value();
    aPose.zMm = theEditors[2]->value();
    aPose.rxDeg = theEditors[3]->value();
    aPose.ryDeg = theEditors[4]->value();
    aPose.rzDeg = theEditors[5]->value();
    return aPose;
}

void DL_RobotCalcDialog::setPoseEditors(QDoubleSpinBox* theEditors[6], const DL_CartesianPose& thePose)
{
    theEditors[0]->setValue(thePose.xMm);
    theEditors[1]->setValue(thePose.yMm);
    theEditors[2]->setValue(thePose.zMm);
    theEditors[3]->setValue(thePose.rxDeg);
    theEditors[4]->setValue(thePose.ryDeg);
    theEditors[5]->setValue(thePose.rzDeg);
}

void DL_RobotCalcDialog::setCurrentJointEditors(const double theAngles[DL_ROBOT_JOINT_COUNT])
{
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
        myCurrentJointEdits[i]->setText(poseValueText(theAngles[i] * rl::math::RAD2DEG));
}

void DL_RobotCalcDialog::setCurrentPoseEditors(const DL_CartesianPose& thePose)
{
    myCurrentPoseEdits[0]->setText(poseValueText(thePose.xMm));
    myCurrentPoseEdits[1]->setText(poseValueText(thePose.yMm));
    myCurrentPoseEdits[2]->setText(poseValueText(thePose.zMm));
    myCurrentPoseEdits[3]->setText(poseValueText(thePose.rxDeg));
    myCurrentPoseEdits[4]->setText(poseValueText(thePose.ryDeg));
    myCurrentPoseEdits[5]->setText(poseValueText(thePose.rzDeg));
}

void DL_RobotCalcDialog::setDefaultRandomRanges(const DL_CartesianPose& thePose)
{
    myRandomRangeEdits[0]->setValue(thePose.xMm - 200.0);
    myRandomRangeEdits[1]->setValue(thePose.xMm + 200.0);
    myRandomRangeEdits[2]->setValue(thePose.yMm - 200.0);
    myRandomRangeEdits[3]->setValue(thePose.yMm + 200.0);
    myRandomRangeEdits[4]->setValue(thePose.zMm - 200.0);
    myRandomRangeEdits[5]->setValue(thePose.zMm + 200.0);
}

void DL_RobotCalcDialog::setResultText(const QString& theText)
{
    myResultEdit->setPlainText(theText);
}

void DL_RobotCalcDialog::applyReport(const DL_RobotCalcReport& theReport)
{
    setResultText(theReport.summary);
    if (theReport.success)
        cacheSolvedAngles(theReport.solvedJointAngles);
    else
        myHasSolvedTarget = false;
    updateUiEnabledState();
}

void DL_RobotCalcDialog::updateUiEnabledState()
{
    QString aMessage;
    const bool isReady = hasValidDocument(&aMessage);
    myReadCurrentButton->setEnabled(isReady);
    myRandomButton->setEnabled(isReady);
    myRandomJointButton->setEnabled(isReady);
    mySolveButton->setEnabled(isReady);
    myAnimateButton->setEnabled(isReady);
    myAnalyzeButton->setEnabled(isReady);
}

bool DL_RobotCalcDialog::solveCurrentTarget(DL_RobotCalcReport* theReport)
{
    if (theReport)
        *theReport = DL_RobotCalcReport();

    QString aMessage;
    if (!hasValidDocument(&aMessage))
    {
        if (theReport) theReport->summary = aMessage;
        myHasSolvedTarget = false;
        updateUiEnabledState();
        return false;
    }

    DL_CartesianPose aPose = poseFromEditors(myTargetPoseEdits);
    DL_RobotCalcReport aReport = robotContext()->analyzeTargetPose(aPose);
    if (aReport.success && myHasSampledJointTarget)
        aReport.summary += QString("\n\n") + sampledJointComparisonText(aReport.solvedJointAngles);
    if (theReport)
        *theReport = aReport;

    if (aReport.success)
        cacheSolvedAngles(aReport.solvedJointAngles);
    else
        myHasSolvedTarget = false;

    updateUiEnabledState();
    return aReport.success;
}

void DL_RobotCalcDialog::cacheSolvedAngles(const double theAngles[DL_ROBOT_JOINT_COUNT])
{
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
        mySolvedAngles[i] = theAngles[i];
    myHasSolvedTarget = true;
}

void DL_RobotCalcDialog::cacheSampledAngles(const double theAngles[DL_ROBOT_JOINT_COUNT])
{
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
        mySampledJointAngles[i] = theAngles[i];
    myHasSampledJointTarget = true;
}

QString DL_RobotCalcDialog::sampledJointComparisonText(const double theSolvedAngles[DL_ROBOT_JOINT_COUNT]) const
{
    if (!myHasSampledJointTarget)
        return QString();

    QStringList aSampleTexts;
    QStringList aDeltaTexts;
    double aMaxDeltaDeg = 0.0;
    for (int i = 0; i < DL_ROBOT_JOINT_COUNT; ++i)
    {
        const double aSampleDeg = mySampledJointAngles[i] * rl::math::RAD2DEG;
        const double aSolvedDeg = theSolvedAngles[i] * rl::math::RAD2DEG;
        const double aDeltaDeg = std::abs(aSolvedDeg - aSampleDeg);
        aMaxDeltaDeg = std::max(aMaxDeltaDeg, aDeltaDeg);
        aSampleTexts << QString("J%1=%2").arg(i + 1).arg(aSampleDeg, 0, 'f', 3);
        aDeltaTexts << QString("dJ%1=%2").arg(i + 1).arg(aDeltaDeg, 0, 'f', 3);
    }

    QStringList aLines;
    aLines << QObject::tr("Sampled joints (deg): %1").arg(aSampleTexts.join(", "));
    aLines << QObject::tr("Sampled-vs-IK delta (deg): %1").arg(aDeltaTexts.join(", "));
    aLines << QObject::tr("Max sampled-vs-IK delta (deg): %1").arg(aMaxDeltaDeg, 0, 'f', 3);
    return aLines.join("\n");
}


