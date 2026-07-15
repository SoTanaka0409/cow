$encUtf8 = New-Object System.Text.UTF8Encoding $true
$file1 = 'c:\Users\Admin\Desktop\progtram\Cow\Source\ResultScene.cpp'
$content1 = [System.IO.File]::ReadAllText($file1)
$content1 = $content1.Replace("mFlag", "flag_")
$content1 = $content1.Replace("graphHandle", "graph_handle")
[System.IO.File]::WriteAllText($file1, $content1, $encUtf8)

$file2 = 'c:\Users\Admin\Desktop\progtram\Cow\Source\TitleScene.cpp'
$content2 = [System.IO.File]::ReadAllText($file2)
$content2 = $content2.Replace("graphHandle", "graph_handle")
$content2 = $content2.Replace("mpCamera", "camera_")
$content2 = $content2.Replace("SCENE_LOADING", "kSceneLoading")
$content2 = $content2.Replace("SCENE_TUTORIAL", "kSceneTutorial")
$content2 = $content2.Replace("SCENE_RULE", "kSceneRule")
[System.IO.File]::WriteAllText($file2, $content2, $encUtf8)
