# -*- coding: utf-8 -*-
"""
纯 Python 脚本：批量识别 COMSOL .mph 文件中 spf（层流）模块的选中域序号
原理：.mph 文件是 ZIP 包，内含 dmodel.xml。直接从 XML 中解析 Physics 节点的 selection/entities 属性。
"""
import zipfile, re, os, glob

MPH_DIR = r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场\mph"
OUT_DIR = os.path.join(MPH_DIR, "层流域识别结果")
os.makedirs(OUT_DIR, exist_ok=True)

def get_spf_entities(mph_path):
    """
    从 .mph 文件中提取 spf（层流）模块的域选择。
    返回 (域名列表, ht域列表) 或 (None, None)
    """
    with zipfile.ZipFile(mph_path, 'r') as z:
        xml = z.read('dmodel.xml').decode('utf-8', errors='ignore')

    def extract_entities(phys_tag):
        """从 XML 中提取指定物理场标签的 entities"""
        # 匹配 <Physics ... tag="spf" ...> ... </Physics>
        pattern = re.compile(
            r'<Physics[^>]*tag="' + phys_tag + r'"[^>]*>.*?</Physics>',
            re.DOTALL
        )
        match = pattern.search(xml)
        if not match:
            return None
        # 从 Physics 块中提取 entities 属性
        ent_match = re.search(r'entities="([^"]+)"', match.group())
        if not ent_match:
            return None
        # 第一个数是 COMSOL 内部编码头（维度+计数），跳过
        parts = ent_match.group(1).split(',')
        header = int(parts[0])
        entities = [int(x) for x in parts[1:]]
        return entities

    spf = extract_entities('spf')
    ht = extract_entities('ht')
    return spf, ht


def main():
    mph_files = sorted(glob.glob(os.path.join(MPH_DIR, "*.mph")))
    print(f"找到 {len(mph_files)} 个 mph 文件")

    summary_path = os.path.join(OUT_DIR, "PYTHON-SPF域识别汇总_python.txt")
    detail_path = os.path.join(OUT_DIR, "PYTHON-SPF域识别详细_python.txt")
    alert_path  = os.path.join(OUT_DIR, "PYTHON-重点标注_超过3个域_python.txt")

    alert_count = 0
    results = []

    for i, mph_path in enumerate(mph_files):
        fname = os.path.basename(mph_path)
        try:
            spf_ents, ht_ents = get_spf_entities(mph_path)
            if spf_ents is None:
                print(f"[{i:3d}] {fname}  ERROR: 未找到 spf")
                results.append((fname, 0, [], "ERROR"))
                continue

            n = len(spf_ents)
            flag = " *** >3个域! ***" if n > 3 else ""
            if n > 3:
                alert_count += 1
            print(f"[{i:3d}] {fname}  spf: {n}个域 {spf_ents} {flag}")
            results.append((fname, n, spf_ents, ht_ents))
        except Exception as e:
            print(f"[{i:3d}] {fname}  ERROR: {e}")
            results.append((fname, 0, [], f"ERROR: {e}"))

    # 写入汇总文件
    with open(summary_path, 'w', encoding='utf-8') as f:
        f.write("SPF层流域识别汇总 (Python解析)\n")
        f.write(f"扫描目录: {MPH_DIR}\n")
        f.write(f"文件总数: {len(mph_files)}\n\n")
        f.write("文件名\t\t\t\t\t\tSPF域数\t选中域\t\t重点标注\n")
        f.write("=" * 90 + "\n")
        for fname, n, ents, ht_ents in results:
            flag = " *** >3个域! ***" if n > 3 else ""
            f.write(f"{fname}\t{n}\t{ents}\t{flag}\n")
        f.write(f"\n处理完成: {len(mph_files)} 个文件, 重点标注: {alert_count} 个\n")

    # 写入详细文件
    with open(detail_path, 'w', encoding='utf-8') as f:
        f.write("SPF层流域识别详细结果 (Python解析)\n\n")
        for fname, n, ents, ht_ents in results:
            f.write(f"[{fname}]\n")
            f.write(f"  SPF域数量: {n}\n")
            f.write(f"  SPF选中域: {ents}\n")
            if ht_ents:
                f.write(f"  HT域数量: {len(ht_ents)}\n")
                f.write(f"  HT选中域: {ht_ents}\n")
            f.write(f"  重点标注: {'是 (>3个域)' if n > 3 else '否'}\n\n")

    # 写入重点标注文件
    with open(alert_path, 'w', encoding='utf-8') as f:
        f.write("重点标注: SPF域数量超过3个的文件 (Python解析)\n\n")
        for fname, n, ents, _ in results:
            if n > 3:
                f.write(f"==> {fname} <==  SPF域数量: {n}\n")
                f.write(f"  选中域: {ents}\n\n")
        f.write(f"\n重点标注总数: {alert_count}\n")

    print(f"\n完成! 结果保存到: {OUT_DIR}")
    print(f"  汇总: {os.path.basename(summary_path)}")
    print(f"  详细: {os.path.basename(detail_path)}")
    print(f"  重点: {os.path.basename(alert_path)}")


if __name__ == '__main__':
    main()
