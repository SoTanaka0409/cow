def fix(s):
    try:
        return s.encode('cp932').decode('utf-8')
    except:
        return s

print(fix('山/地形'))
print(fix('石'))
print(fix('新しい柵'))
print(fix('裁E��'))
