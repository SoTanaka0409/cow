$path = "C:\Users\Admin\.gemini\antigravity\brain\0e47fdf1-e25c-4e86-b182-58fa440958d2\.system_generated\logs\transcript.jsonl"
Get-Content $path -Encoding UTF8 | Where-Object { $_ -match "void Cow_gold::OnTrigger" } | Select-Object -First 1 | Set-Content "C:\Users\Admin\Desktop\progtram\Cow\old_cow_gold.txt"
