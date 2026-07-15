$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  # Floor
  @{ Old = 'mnGraphHandle'; New = 'graph_handle_' },
  @{ Old = 'mVertex';       New = 'vertex_' },

  # Stage
  @{ Old = 'mnModelHandle';     New = 'model_handle_' },
  @{ Old = 'mnCollisionHandle'; New = 'collision_handle_' },

  # Thunder member variables
  @{ Old = 'mWarningTimer';   New = 'warning_timer_' },
  @{ Old = 'mStrikeTimer';    New = 'strike_timer_' },
  @{ Old = 'mState';          New = 'state_' },
  @{ Old = 'mIntervalTimer';  New = 'interval_timer_' },
  @{ Old = 'mActive';         New = 'active_' },
  @{ Old = 'mpThunder';       New = 'thunder_' },
  @{ Old = 'mpWarning';       New = 'warning_' },
  @{ Old = 'mpStun';          New = 'stun_' },
  @{ Old = 'mHasStunned';     New = 'has_stunned_' },
  @{ Old = 'mStunEffectTimer';New = 'stun_effect_timer_' },

  # Thunder::State enum (only apply to Thunder-specific identifiers)
  @{ Old = '\bIDLE\b';    New = 'kIdle' },
  @{ Old = '\bWARNING\b'; New = 'kWarning' },
  @{ Old = '\bSTRIKE\b';  New = 'kStrike' },
  @{ Old = '\bEND\b';     New = 'kEnd' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::Default)
  $newText = $text

  foreach ($r in $replacements) {
    if ($r.Old.StartsWith('\b')) {
      $newText = $newText -creplace $r.Old, $r.New
    } else {
      $regex = '\b' + [regex]::Escape($r.Old) + '\b'
      $newText = $newText -creplace $regex, $r.New
    }
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")
