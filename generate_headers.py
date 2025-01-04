import sys

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, "rb") as f:
    data = f.read()

with open(output_file, "w") as f:
    f.write("unsigned char icon_png[] = {\n")
    f.write(", ".join(f"0x{byte:02x}" for byte in data))
    f.write("\n};\n")
    f.write(f"unsigned int icon_png_len = {len(data)};\n")