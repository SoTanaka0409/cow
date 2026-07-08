$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  @{ Old = 'Tag_animal';          New = 'TagAnimal' },
  @{ Old = 'mntag_animal';        New = 'tag_animal_' },
  @{ Old = 'AnimalMove::none';    New = 'AnimalMove::kNone' },
  @{ Old = 'AnimalMove::Animal_1';New = 'AnimalMove::kAnimal1' },
  @{ Old = 'AnimalMove::Animal_2';New = 'AnimalMove::kAnimal2' },
  @{ Old = 'AnimalMove::Animal_3';New = 'AnimalMove::kAnimal3' },
  @{ Old = 'AnimalMove::Animal_gold'; New = 'AnimalMove::kAnimalGold' },
  @{ Old = 'AnimalMove::Animal_T';New = 'AnimalMove::kAnimalT' },

  @{ Old = 'Tag_cow';             New = 'TagCow' },
  @{ Old = 'mntag_cow';           New = 'tag_cow_' },
  @{ Old = 'CowMove::none';       New = 'CowMove::kNone' },
  @{ Old = 'CowMove::Cow_1';      New = 'CowMove::kCow1' },
  @{ Old = 'CowMove::Cow_2';      New = 'CowMove::kCow2' },
  @{ Old = 'CowMove::Cow_3';      New = 'CowMove::kCow3' },
  @{ Old = 'CowMove::Cow_gold';   New = 'CowMove::kCowGold' },
  @{ Old = 'CowMove::Cow_T';      New = 'CowMove::kCowT' },

  @{ Old = 'mColliderRadius';     New = 'collider_radius_' },
  @{ Old = 'mpCowVm';             New = 'cow_vm_' },
  @{ Old = 'mEffectTimer';        New = 'effect_timer_' },
  @{ Old = 'TutrialVacumFlag';    New = 'tutorial_vacum_flag_' },

  @{ Old = 'Tag_fever';           New = 'TagFever' },
  @{ Old = 'Cow_gold::fever';     New = 'Cow_gold::kFever' },
  @{ Old = 'Cow_gold::Nofever';   New = 'Cow_gold::kNoFever' },
  @{ Old = 'mnFever';             New = 'fever_' },
  @{ Old = 'DeathCount';          New = 'death_count_' },
  @{ Old = 'DeathTimer';          New = 'death_timer_' }
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
