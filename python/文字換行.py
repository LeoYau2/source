with open("input.txt", "r", encoding='utf-8') as f:  # 明确使用 UTF-8 编码
    content = f.read()
    replaced = content.replace("\n", "\\n")
    print(replaced)
