#!/usr/bin/env python3
"""Build viewer.html: one tabbed page that inlines every SVG in diagrams/.

Usage: python3 tools/build_viewer.py   (run from projectDocuments/)
"""
import html
import pathlib
import re

root = pathlib.Path(__file__).resolve().parent.parent
svgs = sorted((root / "diagrams").glob("*.svg"))
order = ["system-layout", "electronics-wiring", "pvc-housing-section"]
svgs.sort(key=lambda p: order.index(p.stem) if p.stem in order else 99)


def title_of(text, fallback):
    m = re.search(r"<title>(.*?)</title>", text, re.S)
    return html.unescape(m.group(1)) if m else fallback


tabs, panels = [], []
for i, p in enumerate(svgs):
    text = p.read_text(encoding="utf-8")
    name = title_of(text, p.stem)
    sel = " active" if i == 0 else ""
    tabs.append(f'<button class="tab{sel}" data-id="{p.stem}">{html.escape(name)}</button>')
    panels.append(
        f'<section class="panel{sel}" id="{p.stem}">{text}'
        f'<p class="file">diagrams/{p.name}</p></section>'
    )

page = f"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Sensor release diagrams</title>
<style>
body{{margin:0;font-family:system-ui,-apple-system,Segoe UI,sans-serif;background:#f4f4f2;color:#222}}
header{{padding:16px 20px 0}}
h1{{font-size:18px;margin:0 0 4px}}
.sub{{font-size:13px;color:#555;margin:0 0 12px}}
.tabs{{display:flex;flex-wrap:wrap;gap:6px;padding:0 20px}}
.tab{{border:1px solid #c9c9c4;background:#fff;border-radius:6px;padding:7px 12px;font-size:13px;cursor:pointer}}
.tab.active{{background:#222;color:#fff;border-color:#222}}
.panel{{display:none;margin:14px 20px 28px;background:#fff;border:1px solid #ddd;border-radius:8px;padding:12px}}
.panel.active{{display:block}}
.panel svg{{width:100%;height:auto;display:block}}
.file{{font-size:12px;color:#777;margin:8px 0 0;font-family:ui-monospace,Menlo,monospace}}
</style></head><body>
<header><h1>Sensor release: current diagrams</h1>
<p class="sub">Current plan as of Nov 8, 2026. Source files are plain SVG in the diagrams/ folder.</p></header>
<div class="tabs">{''.join(tabs)}</div>
{''.join(panels)}
<script>
function show(id){{
  document.querySelectorAll('.tab').forEach(function(t){{t.classList.toggle('active',t.dataset.id===id)}});
  document.querySelectorAll('.panel').forEach(function(p){{p.classList.toggle('active',p.id===id)}});
}}
document.querySelectorAll('.tab').forEach(function(t){{t.addEventListener('click',function(){{show(t.dataset.id);try{{location.hash=t.dataset.id}}catch(e){{}}}})}});
var h=(location.hash||'').slice(1); if(h&&document.getElementById(h)) show(h);
window.addEventListener('hashchange',function(){{var x=(location.hash||'').slice(1); if(x&&document.getElementById(x)) show(x)}});
</script></body></html>
"""
(root / "viewer.html").write_text(page, encoding="utf-8")
print("wrote", root / "viewer.html", f"({len(svgs)} diagrams)")
