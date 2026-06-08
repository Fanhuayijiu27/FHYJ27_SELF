import csv

input_file = r"D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\1三列数据二值化\名字对照表.txt"
output_file = r"D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\1三列数据二值化\名字对照表_modified.txt"

with open(input_file, "r", encoding="utf-8") as fin, \
     open(output_file, "w", encoding="utf-8", newline="") as fout:
    reader = csv.reader(fin, delimiter="\t")
    writer = csv.writer(fout, delimiter="\t")
    header = next(reader)
    writer.writerow(header)
    for row in reader:
        # 第三列(index 2)和第五列(index 4): +0.5 再 -32, 即 -31.5
        row[2] = str(float(row[2]) - 31.5)
        row[4] = str(float(row[4]) - 31.5)
        writer.writerow(row)

print("处理完成，输出文件：", output_file)
