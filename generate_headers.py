import sys

input_file = sys.argv[1]
output_file = sys.argv[2]
try:
    with open(input_file, "rb") as f:
        data = f.read()
except FileNotFoundError:
    print(f"Error: Input file '{input_file}' not found.", file=sys.stderr)
    sys.exit(1)
except IOError as e:
    print(f"Error reading input file: {e}", file=sys.stderr)
    sys.exit(1)
try:
    with open(output_file, "w") as f:
        f.write("#pragma once\n")
        f.write("inline unsigned char icon_png[] = {\n")
        f.write(", ".join(f"0x{byte:02x}" for byte in data))
        f.write("\n};\n")
        f.write(f"inline unsigned int icon_png_len = {len(data)};\n")
except IOError as e:
    print(f"Error writing output file: {e}", file=sys.stderr)
    sys.exit(1)
print("Converting to header was successful")
