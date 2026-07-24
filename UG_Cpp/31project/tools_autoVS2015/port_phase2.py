#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Phase 2: Remove remaining old control references from FeatureSpy260722.cpp"""

cpp_path = r'd:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722\FeatureSpy260722\FeatureSpy260722.cpp'

with open(cpp_path, 'r', encoding='utf-8') as f:
    cpp = f.read()

print(f"Input: {len(cpp)} chars")

# --- 1. Remove face_select01 line from dialogShown_cb ---
old = '\t\t\tface_select0->SetFaceRules(1);\n\t\t\tface_select01->SetFaceRules(1);'
new = '\t\t\tface_select0->SetFaceRules(1);'
if old in cpp:
    cpp = cpp.replace(old, new)
    print("1. dialogShown_cb face_select01 removed (tab version)")
else:
    # Try space version
    # Find the line and remove it
    marker = 'face_select01->SetFaceRules(1);'
    idx = cpp.find(marker)
    if idx > 0:
        # Find start of this line
        line_start = cpp.rfind('\n', 0, idx) + 1
        line_end = cpp.find('\n', idx)
        line = cpp[line_start:line_end]
        cpp = cpp[:line_start] + cpp[line_end+1:]
        print(f"1. dialogShown_cb face_select01 removed (line: {repr(line.strip())})")
    else:
        print("1. face_select01 line NOT FOUND (may already be removed)")

# --- 2. Remove face_select01 handler ---
m_start = 'else if (block == face_select01)'
if m_start in cpp:
    s = cpp.find(m_start)
    # Find the closing brace: look for the next "}\n" that's followed by "else if" at the same indent
    # The handler ends at the matching brace before the next else-if
    m_next = 'else if (block == button03)'
    e = cpp.find(m_next, s)
    if e > s:
        # Also remove the whitespace/newline between them
        cpp = cpp[:s] + cpp[e:]
        print(f"2. face_select01 handler removed ({e-s} chars)")
    else:
        print("2. face_select01 end marker not found")
else:
    print("2. face_select01 handler NOT FOUND")

# --- 3. Remove button03 handler ---
m_start = 'else if (block == button03)'
if m_start in cpp:
    s = cpp.find(m_start)
    m_next = 'else if (block == list_box01)'
    e = cpp.find(m_next, s)
    if e > s:
        cpp = cpp[:s] + cpp[e:]
        print(f"3. button03 handler removed ({e-s} chars)")
    else:
        print("3. button03 end marker not found")
else:
    print("3. button03 handler NOT FOUND")

# --- 4. Remove list_box01 handler ---
# The list_box01 handler is the last else-if before the catch
m_start = 'else if (block == list_box01)'
if m_start in cpp:
    s = cpp.find(m_start)
    # Find the closing brace pattern
    m_next = '}\n    catch (exception& ex)'
    e = cpp.find(m_next, s)
    if e > s:
        # Keep the "}\n    catch" part (the closing of update_cb's if-else chain)
        cpp = cpp[:s] + cpp[e+1:]  # +1 to keep the '}' that closes the if-else chain
        print(f"4. list_box01 handler removed ({e-s} chars)")
    else:
        print("4. list_box01 end marker not found")
else:
    print("4. list_box01 handler NOT FOUND")

# --- Verify no remaining old control references ---
for name in ['face_select01', 'button03', 'list_box01']:
    count = cpp.count(name)
    if count > 0:
        print(f"WARNING: {name} still appears {count} times in file!")
        # These might be in comments or the custom functions section
    else:
        print(f"CLEAN: {name} fully removed")

print(f"\nOutput: {len(cpp)} chars")

with open(cpp_path, 'w', encoding='utf-8') as f:
    f.write(cpp)
print("Saved. Phase 2 DONE")
