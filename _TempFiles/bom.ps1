$content = [System.IO.File]::ReadAllText('Source/Scene3D.cpp')
[System.IO.File]::WriteAllText('Source/Scene3D.cpp', $content, [System.Text.Encoding]::UTF8)
