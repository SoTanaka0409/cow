$utf8 = New-Object System.Text.UTF8Encoding $true
$content = [System.IO.File]::ReadAllText('c:\Users\Admin\Desktop\progtram\Cow\Source\TitleScene.cpp')
[System.IO.File]::WriteAllText('c:\Users\Admin\Desktop\progtram\Cow\Source\TitleScene.cpp', $content, $utf8)

$content2 = [System.IO.File]::ReadAllText('c:\Users\Admin\Desktop\progtram\Cow\Source\Rule.cpp')
[System.IO.File]::WriteAllText('c:\Users\Admin\Desktop\progtram\Cow\Source\Rule.cpp', $content2, $utf8)
