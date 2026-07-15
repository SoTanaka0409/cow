$text = [System.IO.File]::ReadAllText("tut_tmp.bin", [System.Text.Encoding]::GetEncoding(932)); ($text -split "\r?\n") | Where-Object { $_ -match "Enter" }
