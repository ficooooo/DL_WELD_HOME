from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET


ROOT = Path(r"D:\DL_WELD_HOME")
COMMON_PATH = ROOT / r"src\dlOcctMdiFrame\Common-string.ts"
DL_PATH = ROOT / r"src\DL_RobotConfig\DL_RobotConfig-string.ts"


def git_show(path: str) -> str:
    return subprocess.check_output(
        ["git", "-C", str(ROOT), "show", f"HEAD:{path}"],
        text=True,
        encoding="utf-8",
    )


def parse_ts_from_git(path: str) -> ET.ElementTree:
    return ET.ElementTree(ET.fromstring(git_show(path)))


def find_or_create_context(root: ET.Element, name: str) -> ET.Element:
    for ctx in root.findall("context"):
        node = ctx.find("name")
        if node is not None and node.text == name:
            return ctx
    ctx = ET.SubElement(root, "context")
    node = ET.SubElement(ctx, "name")
    node.text = name
    return ctx


def set_translation(context: ET.Element, source: str, translation: str) -> None:
    for msg in context.findall("message"):
        src = msg.find("source")
        if src is not None and src.text == source:
            tr = msg.find("translation")
            if tr is None:
                tr = ET.SubElement(msg, "translation")
            tr.text = translation
            return
    msg = ET.SubElement(context, "message")
    src = ET.SubElement(msg, "source")
    src.text = source
    tr = ET.SubElement(msg, "translation")
    tr.text = translation


def save_ts(tree: ET.ElementTree, path: Path, language: str) -> None:
    root = tree.getroot()
    root.set("language", language)
    ET.indent(tree, space="    ")
    xml = ET.tostring(root, encoding="utf-8", xml_declaration=True).decode("utf-8")
    if "<!DOCTYPE TS>" not in xml:
        xml = xml.replace("?>", "?>\n<!DOCTYPE TS>", 1)
    path.write_text(xml, encoding="utf-8", newline="\n")


common_tree = parse_ts_from_git("src/dlOcctMdiFrame/Common-string.ts")
common_root = common_tree.getroot()
common_qobj = find_or_create_context(common_root, "QObject")

common_map = {
    "TBR_ROTATION": "动态旋转",
    "MNU_GLOBALPAN": "全局平移(&G)",
    "MNU_ROTATION": "动态旋转(&R)",
    "MNU_AXO": "轴测(&A)",
    "MNU_NEW": "新建(&N)",
    "MNU_PAN": "动态平移(&P)",
    "MNU_TOP": "上视(&T)",
    "MNU_TOOL_SHAD": "着色(&S)",
    "MNU_TOOL_WIRE": "线框(&W)",
    "TBR_BOTTOM": "下视",
    "MNU_BOTTOM": "下视(&B)",
    "BTN_BRONZE": "青铜",
    "MNU_BRONZE": "青铜(&B)",
    "TBR_BRONZE": "青铜",
    "BTN_COPPER": "铜",
    "MNU_COPPER": "铜(&C)",
    "TBR_COPPER": "铜",
    "MNU_TOOL_COLOR": "颜色(&C)",
    "MNU_TOOL_MATER": "材质(&M)",
    "TBR_FITALL": "适配全部",
    "MNU_FITALL": "适配全部(&F)",
    "MNU_BACK": "后视(&B)",
    "MNU_FILE": "文件(&F)",
    "MNU_PREFERENCES": "首选项(&P)",
    "MNU_USE_VBO": "使用 VBO(&V)",
    "MNU_GOLD": "金(&G)",
    "MNU_HELP": "帮助(&H)",
    "MNU_LEFT": "左视(&L)",
    "MNU_QUIT": "退出(&Q)",
    "MNU_VIEW": "视图(&V)",
    "MNU_ZOOM": "动态缩放(&Z)",
    "MNU_HLROFF": "隐藏线关闭(&O)",
    "TBR_HLROFF": "隐藏线关闭",
    "MNU_TOOL_TRANS": "透明度(&T)",
    "MNU_TOOL_SHADOWS": "阴影(&S)",
    "MNU_TOOL_RAYTRACING": "光线追踪(&R)",
    "MNU_TOOL_REFLECTIONS": "反射(&R)",
    "MNU_TOOL_ANTIALIASING": "抗锯齿(&A)",
    "BTN_BRASS": "黄铜",
    "BTN_PEWTER": "白镴",
    "TBR_PEWTER": "白镴",
    "MNU_PEWTER": "白镴(&P)",
    "MNU_SILVER": "银(&S)",
    "BTN_OK": "确定",
    "TBR_SILVER": "银",
    "BTN_SILVER": "银",
    "INF_NULL_SHAPE": "请选择要导出的形体。",
    "MNU_WINDOW": "窗口(&W)",
    "TBR_WINDOW": "窗口",
    "BTN_PLASTIC": "塑料",
    "BTN_PLASTER": "石膏",
    "MNU_PLASTIC": "塑料(&P)",
    "MNU_PLASTER": "石膏(&P)",
    "TBR_PLASTIC": "塑料",
    "TBR_PLASTER": "石膏",
    "MNU_TOOL_BAR": "工具栏(&T)",
    "MNU_TOOL_DEL": "删除(&D)",
    "TBR_BACK": "后视",
    "TBR_GOLD": "金",
    "TBR_LEFT": "左视",
    "TBR_QUIT": "退出",
    "TBR_VIEW": "视图",
    "TBR_ZOOM": "动态缩放",
    "MNU_CH_BACK": "更改背景(&B)",
    "MNU_CH_ENV_MAP": "环境贴图(&E)",
    "TBR_CH_BACK": "更改背景",
    "TBR_TOOL_COLOR": "颜色",
    "TBR_TOOL_MATER": "材质",
    "TBR_TOOL_RAYTRACING": "启用光线追踪",
    "TBR_TOOL_SHADOWS": "启用阴影",
    "TBR_TOOL_REFLECTIONS": "启用反射",
    "TBR_TOOL_ANTIALIASING": "启用抗锯齿",
    "TBR_TOOL_BAR": "工具栏",
    "TBR_TOOL_DEL": "删除",
    "TBR_TOOL_TRANS": "透明度",
    "TBR_ABOUT": "关于",
    "TBR_BRASS": "黄铜",
    "TBR_CLOSE": "关闭",
    "TBR_FRONT": "前视",
    "TBR_HLRON": "隐藏线显示",
    "TBR_MATER": "材质",
    "TBR_RIGHT": "右视",
    "TBR_RESET": "重置",
    "TBR_AXO": "轴测",
    "TBR_NEW": "新建",
    "TBR_PAN": "动态平移",
    "TBR_TOP": "上视",
    "TIT_INF": "信息",
    "BTN_GOLD": "金",
    "TBR_WINDOW_NEW3D": "新建 3D 视图",
    "MNU_WINDOW_NEW3D": "新建 3D 视图",
    "MNU_STATUS_BAR": "状态栏(&S)",
    "TIT_ERROR": "错误",
    "TBR_FITAREA": "窗口缩放",
    "MNU_FITAREA": "窗口缩放(&Z)",
    "TBR_GLOBALPAN": "全局平移",
    "MNU_ABOUT": "关于(&A)",
    "MNU_BRASS": "黄铜(&B)",
    "MNU_CLOSE": "关闭(&C)",
    "MNU_FRONT": "前视(&F)",
    "MNU_HLRON": "隐藏线显示(&H)",
    "MNU_MATER": "材质(&M)",
    "MNU_RIGHT": "右视(&R)",
    "MNU_RESET": "重置(&R)",
    "INF_ABOUT": "基于 Qt 的 Open CASCADE Technology 教学示例程序",
    "INF_READY": "就绪",
    "TBR_TOOL_SHAD": "着色",
    "TBR_TOOL_WIRE": "线框",
    "TBR_WINDOW_TILE": "平铺",
    "TBR_STATUS_BAR": "状态栏",
    "MNU_WINDOW_CASCADE": "层叠",
    "TBR_WINDOW_CASCADE": "层叠",
    "MNU_WINDOW_TILE": "平铺(&T)",
}
for source, translation in common_map.items():
    set_translation(common_qobj, source, translation)

extra_common = {
    "ApplicationCommonWindow": {
        "File Operations": "文件操作",
        "View Operations": "视图操作",
        "Shape Operations": "形体显示",
        "Ray-tracing Options": "光线追踪选项",
    },
    "DLMUI_FileWnd": {
        "File Operations": "文件操作",
    },
    "DLMUI_ViewOps": {
        "View Operations": "视图操作",
    },
    "DLMUI_ModelSkin": {
        "Shape Operations": "形体显示",
    },
    "DLMUI_Raytrace": {
        "Ray-tracing Options": "光线追踪选项",
    },
    "DLMUI_BackgroundSet": {
        "Open File": "打开文件",
        "All Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tga)": "所有图像文件 (*.bmp *.gif *.jpg *.jpeg *.png *.tga)",
    },
}
for ctx_name, mapping in extra_common.items():
    ctx = find_or_create_context(common_root, ctx_name)
    for source, translation in mapping.items():
        set_translation(ctx, source, translation)

save_ts(common_tree, COMMON_PATH, "zh_CN")

dl_tree = parse_ts_from_git("src/DL_RobotConfig/DL_RobotConfig-string.ts")
dl_root = dl_tree.getroot()
dl_qobj = find_or_create_context(dl_root, "QObject")

dl_map = {
    "TIT_SAMPLE": "DL_RobotConfig",
    "INF_MAKE_BOTTLE": "正在生成示例模型...",
    "TBR_MAKEBOT": "示例模型",
    "MNU_ROBOT_OPEN": "打开(&O)...",
    "TBR_ROBOT_OPEN": "打开/载入机器人或总装 STEP",
    "INF_ROBOT_OPEN": "选择 Top.xml 载入机器人，或选择 STEP 预览总装",
    "INF_DONE": "完成",
    "INF_JOINT_FWORD": "当前关节前进",
    "TBR_JOINT_FWORD": "前进",
    "INF_JOINT_BWORD": "当前关节后退",
    "TBR_JOINT_BWORD": "后退",
    "INF_JOINT_SELECT": "循环选择关节",
    "TBR_JOINT_SELECT": "选择关节",
    "INF_ROBOT_RESET": "重置机器人所有关节为 0",
    "TBR_ROBOT_RESET": "重置机器人",
    "TBR_ROBOT_CALC": "验证计算",
    "INF_ROBOT_CALC": "执行正逆解闭环计算并对比误差",
    "TBR_ROBOT_DISAS": "拆分机械臂",
    "INF_ROBOT_DISAS": "拆分、导出、显示机械臂",
    "TBR_ROBOT_WRITE": "编写 XML",
    "INF_ROBOT_WRITE": "输入参数并生成机械臂 XML 文件",
    "TIT_ABOUT": "DL_RobotConfig",
    "Robot Import Operations": "机器人导入操作",
    "Robot Control Operations": "机器人控制操作",
    "Tips": "提示",
    "Done": "完成",
    "Please create an OCC 3D window first.": "请先创建一个 OCC 三维窗口。",
    "Please open an OCC 3D window first.": "请先打开一个 OCC 三维窗口。",
    "Single STEP preview loaded. Disassemble and Write XML are enabled.": "已载入单个 STEP 预览，拆分和写 XML 已启用。",
    "Current Joint: J#%1 of %2": "当前关节：J#%1 / %2",
    "Robot Calc": "机器人计算",
    "Current State": "当前状态",
    "Target Pose": "目标位姿",
    "Random Range": "随机范围",
    "X min(mm)": "X 最小值(mm)",
    "X max(mm)": "X 最大值(mm)",
    "Y min(mm)": "Y 最小值(mm)",
    "Y max(mm)": "Y 最大值(mm)",
    "Z min(mm)": "Z 最小值(mm)",
    "Z max(mm)": "Z 最大值(mm)",
    "Read Current Pose": "读取当前位姿",
    "Random XYZ": "随机 XYZ",
    "Random Joint Sample": "随机关节采样",
    "Solve IK": "求逆解",
    "Animate To Target": "动画移动到目标",
    "Analyze Current Pose": "分析当前位姿",
    "Close": "关闭",
    "Result Summary": "结果摘要",
    "Please open a loaded robot document first.": "请先打开一个已载入机器人的文档。",
    "Current document is invalid. Please reopen a robot document.": "当前文档已失效，请重新打开机器人文档。",
    "Current pose loaded.": "已读取当前位姿。",
    "Invalid random range: each min must be less than or equal to max.": "随机范围无效：每个最小值必须小于或等于最大值。",
    "No reachable pose found inside the current random range.": "在当前随机范围内未找到可达位姿。",
    "A reachable random target was generated. Run IK or animate to target.": "已生成一个可达的随机目标。请执行逆解或直接播放动画。",
    "Target pose was generated from FK of a random joint sample.": "该目标位姿由一组随机关节角经过正解生成。",
    "You can run Solve IK again or animate directly.": "你可以再次执行逆解，或直接播放动画。",
    "Animation failed.": "动画执行失败。",
    "Animation completed.": "动画执行完成。",
    "Current document was closed. Please reopen a robot document.": "当前文档已关闭，请重新打开机器人文档。",
    "No active robot document is available.": "当前没有可用的活动机器人文档。",
    "The current document has no loaded robot.": "当前文档尚未载入机器人。",
    "Robot context is unavailable.": "机器人上下文不可用。",
    "Sampled joints (deg): %1": "采样关节角(deg)：%1",
    "Sampled-vs-IK delta (deg): %1": "采样值与 IK 解差值(deg)：%1",
    "Max sampled-vs-IK delta (deg): %1": "采样值与 IK 解最大差值(deg)：%1",
    "Robot is not loaded.": "机器人尚未载入。",
    "Target pose is unreachable.": "目标位姿不可达。",
    "Target pose IK succeeded.": "目标位姿逆解成功。",
    "No solvable target pose found from random joint sampling.": "随机关节采样未找到可求解的目标位姿。",
    "Random joint sample succeeded after %1 attempt(s).": "随机关节采样在第 %1 次尝试后成功。",
    "Current pose (mm/deg): X=%1, Y=%2, Z=%3, Rx=%4, Ry=%5, Rz=%6": "当前位姿(mm/deg)：X=%1, Y=%2, Z=%3, Rx=%4, Ry=%5, Rz=%6",
    "Target pose (mm/deg): X=%1, Y=%2, Z=%3, Rx=%4, Ry=%5, Rz=%6": "目标位姿(mm/deg)：X=%1, Y=%2, Z=%3, Rx=%4, Ry=%5, Rz=%6",
    "Solved joints (deg): %1": "求解关节角(deg)：%1",
    "Warning: at least one joint is within 5 deg of its limit.": "警告：至少有一个关节距离限位不足 5 度。",
    "Warning: at least one joint changes more than 45 deg from the current pose.": "警告：至少有一个关节相对当前位姿变化超过 45 度。",
    "No limit or large-motion warning was triggered.": "未触发限位或大幅运动警告。",
    "Choose Top.xml or STEP file": "选择 Top.xml 或 STEP 文件",
    "Top XML (*.xml);;STEP Files (*.stp *.step)": "Top XML (*.xml);;STEP 文件 (*.stp *.step)",
    "Please choose a Top XML file or a STEP file.": "请选择 Top XML 文件或 STEP 文件。",
    "Can not open Top.xml:\n%1": "无法打开 Top.xml：\n%1",
    "Top.xml parse failed.": "Top.xml 解析失败。",
    "Please choose a valid Top.xml file.": "请选择有效的 Top.xml 文件。",
    "Can not build robot.xml from Top.xml.": "无法根据 Top.xml 构建 robot.xml。",
    "Load robot failed: %1": "载入机器人失败：%1",
    "robot.xml missing or invalid.": "robot.xml 缺失或无效。",
    "Load STEP preview failed.": "载入 STEP 预览失败。",
    "Choose stp or step": "选择 stp 或 step 文件",
    "STEP Files (*.stp *.step)": "STEP 文件 (*.stp *.step)",
    "Write Robot Xml": "写入机器人 XML",
    "Case1:Straight (W in X)": "方案1：直形（W 朝向 X）",
    "Case2:Bend (W in Z)": "方案2：弯形（W 朝向 Z）",
    "Manufacturer": "制造商",
    "Model": "型号",
    "Tcp X(mm)": "Tcp X(mm)",
    "Tcp Y(mm)": "Tcp Y(mm)",
    "Tcp Z(mm)": "Tcp Z(mm)",
    "Tcp Rx(Degree)": "Tcp Rx(度)",
    "Tcp Ry(Degree)": "Tcp Ry(度)",
    "Tcp Rz(Degree)": "Tcp Rz(度)",
    "Min(Degree)": "最小值(度)",
    "Max(Degree)": "最大值(度)",
    "MaxSpeed": "最大速度",
    "Joint %1:": "关节 %1：",
    "Create Top.xml + robot.xml": "创建 Top.xml + robot.xml",
    "Save to...": "保存到...",
    "robot.xml build failed.": "robot.xml 构建失败。",
    "Top.xml and robot.xml created!": "Top.xml 和 robot.xml 已创建！",
}
for source, translation in dl_map.items():
    set_translation(dl_qobj, source, translation)

save_ts(dl_tree, DL_PATH, "zh_CN")
print("REBUILD_TRANSLATIONS_OK")
