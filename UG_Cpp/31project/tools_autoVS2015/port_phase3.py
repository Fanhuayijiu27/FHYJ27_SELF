#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Phase 3: Remove list_box01 references and dead custom functions"""

cpp_path = r'd:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722\FeatureSpy260722\FeatureSpy260722.cpp'

with open(cpp_path, 'r', encoding='utf-8') as f:
    cpp = f.read()

print(f"Input: {len(cpp)} chars")

# --- 1. Remove list_box01 handler from update_cb ---
old_handler = '''\t\telse if (block == list_box01)
\t\t{
\t\t\t//---------Enter your code here-----------
\t\t}'''
if old_handler in cpp:
    cpp = cpp.replace(old_handler, '')
    print("1. list_box01 handler removed (tab version)")
else:
    # Try space version
    old_handler2 = 'else if (block == list_box01)\n\t\t\t{\n\t\t\t\t//---------Enter your code here-----------\n\t\t\t}'
    if old_handler2 in cpp:
        cpp = cpp.replace(old_handler2, '')
        print("1. list_box01 handler removed (space version)")
    else:
        print("1. list_box01 handler NOT FOUND - searching...")
        idx = cpp.find('else if (block == list_box01)')
        if idx > 0:
            end_idx = cpp.find('\n', cpp.find('}', idx))
            # Find proper end
            end_block = cpp.find('\t\t}\n', idx)
            if end_block < 0:
                end_block = cpp.find('}\n', idx + 40)
            if end_block > 0:
                block = cpp[idx:end_block+3]
                print(f"  Found block: {repr(block[:100])}")
                cpp = cpp[:idx] + cpp[end_block+3:]
                print(f"1. list_box01 handler removed manually")
        else:
            print("1. list_box01 handler already removed")

# --- 2. Remove/comment FeatureSearchResultDisplay function ---
# This function uses list_box01 which no longer exists
# It was only called from button03 (already removed)
func_start = cpp.find('void FeatureSpy260722::FeatureSearchResultDisplay')
if func_start > 0:
    # Find the end of this function
    # Look for the next "void FeatureSpy260722::" or "int FeatureSpy260722::"
    next_func = cpp.find('\n\n\nint FeatureSpy260722::GetDefaultColor', func_start)
    if next_func < 0:
        next_func = cpp.find('\nint FeatureSpy260722::GetDefaultColor', func_start)
    if next_func > func_start:
        old_func = cpp[func_start:next_func]
        cpp = cpp[:func_start] + cpp[next_func:]
        print(f"2. FeatureSearchResultDisplay function removed ({len(old_func)} chars)")
    else:
        print("2. FeatureSearchResultDisplay end not found")
else:
    print("2. FeatureSearchResultDisplay NOT FOUND")

# Also remove the declaration from the class header? No, leave it in .hpp as it won't hurt

# --- Verify ---
for name in ['list_box01', 'face_select01', 'button03']:
    count = cpp.count(name)
    if count > 0:
        print(f"WARNING: {name} still appears {count} times!")
        # Show context
        idx = cpp.find(name)
        if idx > 0:
            line_start = cpp.rfind('\n', 0, idx) + 1
            line_end = cpp.find('\n', idx)
            print(f"  First: line ~{cpp[:idx].count(chr(10))+1}: {cpp[line_start:line_end].strip()[:80]}")
    else:
        print(f"CLEAN: {name} fully removed")

print(f"\nOutput: {len(cpp)} chars")

with open(cpp_path, 'w', encoding='utf-8') as f:
    f.write(cpp)
print("Saved. Phase 3 DONE")
