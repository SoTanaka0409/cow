$files = @("AnimalMove.cpp", "Camera.cpp", "CowManager.cpp", "CowMove.cpp", "DebugCamera.cpp", "GameManager.cpp", "LoadingScene.cpp", "LoadingScene.h", "Main.cpp", "Master.h", "ObjectManager.cpp", "Player3D.cpp", "Player3D.h", "ResultScene.cpp", "ResultScene.h", "Rule.cpp", "Scene.cpp", "Scene.h", "Scene3D.cpp", "Scene3D.h", "SceneManager.cpp", "SceneManager.h", "ServiceLocator.cpp", "Tatumaki.cpp", "Texture.cpp", "Texture.h", "Thunder.cpp", "TitleScene.cpp", "TitleScene.h", "TutorialScene.cpp", "TutorialScene.h")
$utf8bom = New-Object System.Text.UTF8Encoding $True
foreach ($fname in $files) {
    $file = "Source\" + $fname
    # get original text from git
    $origText = git show origin/sou:Source/$fname
    $origTextStr = [string]::Join("`n", $origText)
    
    # regex to match "Resource/..."
    $regex = '"Resource/[^"]+"'
    $origMatches = [regex]::Matches($origTextStr, $regex)
    
    if ($origMatches.Count -gt 0) {
        $currText = [System.IO.File]::ReadAllText($file, [System.Text.Encoding]::UTF8)
        $currMatches = [regex]::Matches($currText, $regex)
        
        if ($origMatches.Count -eq $currMatches.Count) {
            # replace backwards to not mess up indices
            for ($i = $origMatches.Count - 1; $i -ge 0; $i--) {
                $o = $origMatches[$i].Value
                $c = $currMatches[$i]
                $currText = $currText.Remove($c.Index, $c.Length).Insert($c.Index, $o)
            }
            [System.IO.File]::WriteAllText($file, $currText, $utf8bom)
            Write-Host ("Fixed strings in " + $fname)
        } else {
            Write-Host ("Mismatch in " + $fname + " (" + $origMatches.Count + " vs " + $currMatches.Count + ")")
        }
    }
}
Write-Host "Done!"
