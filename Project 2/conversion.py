def convert_tbl_data_to_csv_format_no_header(filename, path=""):
    if path and not path.endswith(('/', '\\')):
        path += '/'
    print(f"Processing {filename}.tbl")

    with open(f"{path}{filename}.csv", "w+") as csv_file:
        with open(f"{path}{filename}.tbl", "r") as tbl_file:
            for line in tbl_file:
                line = line.rstrip('\n').rstrip('|')
                csv_file.write(line + "\n")



path = "./data/"
filenames = ["customer","lineitem","nation","orders","part","partsupp","region","supplier"]

for filename in filenames:
    convert_tbl_data_to_csv_format_no_header(filename, path)
