# Cleanup Notes - File Redundancy Analysis

## Summary

With the new unified build system and embedded component library, many files are now redundant.

## Redundant Files (Can be Removed)

### 1. Old Makefiles (Replaced by `src/Makefile`)

```bash
src/genie_lamp/Makefile          # Replaced by src/Makefile target
src/elxnk/Makefile               # Replaced by src/Makefile target
```

**Keep:** `resources/rmkit/Makefile` (needed for lamp build)

### 2. Runtime SVG Processing Scripts (Replaced by Embedded Library)

```bash
scripts/svg2lamp.sh              # Build-time: tools/svg2header.py
scripts/font_render.sh           # Embedded in component_library.h
scripts/component_library.sh     # Embedded in component_library.h
scripts/test_render_all.sh       # No longer needed
scripts/ui_state.sh              # KEEP - still used by gestures
```

**Note:** `ui_state.sh` is still needed for UI state management

### 3. Old Deployment Script (Replaced by `make deploy`)

```bash
scripts/deploy_elxnk.sh          # Replaced by `cd src && make deploy`
```

### 4. Outdated Documentation

```bash
QUICKSTART.md                    # Outdated, info now in README.md
ELXNK_INTEGRATION.md             # Outdated, info now in README.md
CONTEXT_DATASET.md               # Legacy context file
ESSENTIALS_DEPLOYMENT.md         # Outdated deployment guide
UI_SYSTEM_README.txt             # Outdated UI docs
FONT_GUIDE.md                    # Outdated, fonts now embedded
RM2FB_NOTES.md                   # Not needed (no rm2fb dependency)
```

### 5. Duplicate Config Files

```bash
resources/genie_lamp/genie_lamp.conf    # Duplicate
resources/genie_lamp/lamp_test.conf     # Duplicate
resources/genie_lamp/ui.conf            # Duplicate
src/genie_lamp/genie_lamp.conf          # Keep as reference
src/genie_lamp/lamp_test.conf           # Remove (test only)
```

**Keep:** `src/genie_lamp/ui.conf` (deployed to /opt/etc/genie_ui.conf)

## Files to Keep

### Essential Source Files

```
src/
├── Makefile                     # ✓ Unified build system
├── elxnk/
│   ├── elxnk_main.cpp          # ✓ Main controller
│   └── component_library.h     # ✓ Auto-generated
├── genie_lamp/
│   ├── main.cpp                # ✓ Gesture detector
│   └── ui.conf                 # ✓ Gesture config
└── config/
    ├── elxnk.conf              # ✓ System config
    └── elxnk.service           # ✓ Systemd service

scripts/
└── ui_state.sh                 # ✓ Still used at runtime

tools/
└── svg2header.py               # ✓ Build tool

assets/
├── components/                 # ✓ Build-time sources
└── font/                       # ✓ Build-time sources

resources/rmkit/                # ✓ Needed for lamp build
```

### Documentation

```
README.md                       # ✓ Primary docs (updated)
LICENSE                         # ✓ Keep if exists
```

## Cleanup Commands

### Remove Redundant Files

```bash
# Backup first
mkdir -p .archive
mv QUICKSTART.md ELXNK_INTEGRATION.md CONTEXT_DATASET.md \
   ESSENTIALS_DEPLOYMENT.md UI_SYSTEM_README.txt \
   FONT_GUIDE.md RM2FB_NOTES.md .archive/ 2>/dev/null

# Remove old Makefiles
rm -f src/genie_lamp/Makefile src/elxnk/Makefile

# Remove redundant scripts
rm -f scripts/svg2lamp.sh scripts/font_render.sh \
      scripts/component_library.sh scripts/test_render_all.sh \
      scripts/deploy_elxnk.sh

# Remove duplicate configs
rm -rf resources/genie_lamp/
rm -f src/genie_lamp/lamp_test.conf
```

### Verify Cleanup

```bash
# Should still work
cd src
make clean
make all
make library
```

## File Count Reduction

**Before:**
- ~90 total files
- 6 Makefiles
- 5+ bash scripts
- 8+ documentation files
- 54 SVG assets

**After:**
- ~70 files (20 fewer)
- 2 Makefiles (src/Makefile + rmkit/Makefile)
- 2 bash scripts (ui_state.sh + ui tools if needed)
- 1 documentation file (README.md)
- 54 SVG assets (build-time only)
- 1 Python tool (svg2header.py)

## Benefits

✅ **Simpler Structure:** One Makefile, one README
✅ **No Runtime Dependencies:** All data embedded
✅ **Faster Deployment:** `make deploy` handles everything
✅ **Less Confusion:** Clear what's needed vs legacy
✅ **Smaller Footprint:** Fewer files on reMarkable

## Migration Path

1. ✅ Create unified src/Makefile
2. ✅ Generate component_library.h with embedded data
3. ✅ Update README.md with concise docs
4. 🔄 Remove redundant files (this step)
5. Test deployment: `cd src && make deploy`
6. Archive old docs for reference
