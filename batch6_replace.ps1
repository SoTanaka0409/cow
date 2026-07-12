$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  # ObjectManager member variables
  @{ Old = 'mObject3DList'; New = 'object3d_list_' },
  @{ Old = 'mObject2DList'; New = 'object2d_list_' },
  @{ Old = 'mTagCache3D';   New = 'tag_cache_3d_' },
  @{ Old = 'mTagCache2D';   New = 'tag_cache_2d_' },

  # CowMove getter rename (used in CowManager.h and elsewhere)
  @{ Old = 'GetTag_cow';    New = 'GetTagCow' },
  # AnimalMove getter rename
  @{ Old = 'GetTag_animal'; New = 'GetTagAnimal' },

  # SetTag_cow / SetTag_animal
  @{ Old = 'SetTag_cow';    New = 'SetTagCow' },
  @{ Old = 'SetTag_animal'; New = 'SetTagAnimal' },

  # CowManager member
  @{ Old = 'mfever';        New = 'is_fever' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::Default)
  $newText = $text

  foreach ($r in $replacements) {
    $regex = '\b' + [regex]::Escape($r.Old) + '\b'
    $newText = $newText -creplace $regex, $r.New
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")
