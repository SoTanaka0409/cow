$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  @{ Old = 'mvRotation';              New = 'rotation_' },
  @{ Old = 'mbDeleteFlag';            New = 'delete_flag_' },
  @{ Old = 'mnTag';                   New = 'tag_' },
  @{ Old = 'mbDrawFlag';              New = 'draw_flag_' },

  @{ Old = 'None2D';                  New = 'kNone2d' },
  @{ Old = 'Tag2D_Dino1';             New = 'kTag2dDino1' },
  @{ Old = 'Tag2D_Dino2';             New = 'kTag2dDino2' },

  @{ Old = 'None3D';                  New = 'kNone3d' },
  @{ Old = 'Tag3D_Stage';             New = 'kTag3dStage' },
  @{ Old = 'Tag3D_player';            New = 'kTag3dPlayer' },
  @{ Old = 'Tag3D_Enemy';             New = 'kTag3dEnemy' },
  @{ Old = 'Tag3D_Cow';               New = 'kTag3dCow' },
  @{ Old = 'Tag3D_Wall';              New = 'kTag3dWall' },
  @{ Old = 'Tag3D_Bait';              New = 'kTag3dBait' },
  @{ Old = 'Tag3D_Animal';            New = 'kTag3dAnimal' },

  @{ Old = 'mvOldPosition';           New = 'old_position_' },
  @{ Old = 'mpCapsuleCollider';       New = 'capsule_collider_' },
  @{ Old = 'mfRadius';                New = 'radius_' },
  @{ Old = 'mfCurrentCameraDistance'; New = 'current_camera_distance_' },

  @{ Old = 'mpParentObject';          New = 'parent_object_' },
  @{ Old = 'mvPosition2';             New = 'position2_' },
  @{ Old = 'mCollisionList';          New = 'collision_list_' },

  @{ Old = 'mColliderList';           New = 'collider_list_' },
  @{ Old = 'Instance';                New = 'instance_' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $content = Get-Content $file.FullName -Encoding Default
  $text = $content -join "`r`n"
  $newText = $text

  foreach ($r in $replacements) {
    # -creplace specifies case-sensitive regex replacement
    # we use \b to match whole words so we don't accidentally replace substrings
    $regex = '\b' + $r.Old + '\b'
    $newText = $newText -creplace $regex, $r.New
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")
