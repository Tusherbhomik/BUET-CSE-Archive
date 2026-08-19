# BUET CSE Archive

A single, structured home for coursework, lecture slides, sessional projects, and reference
material from the BUET CSE curriculum, consolidated from 14 previously scattered GitHub
repositories plus the `BUET_CSE_ACADEMICS` course-materials dump.

This repo was assembled by copying content from the original repos/folders (no file content was
modified), organizing it by course level, and removing duplicate/regenerable files along the way.
Each original repo is still available on GitHub with its full commit history — see the mapping
table below.

## Structure

Top level is split by **Theory** (lecture course) vs **Sessional** (lab/project course), since BUET
pairs each theory course with a same-numbered-ish sessional that has separate, distinct content.
Each side is then organized by level.

```
Theory/
  level-1/  CSE-101-Structured-Programming-Language/, CSE-103-Discrete-Mathematics/,
            CSE-107-Object-Oriented-Programming/
  level-2/  CSE-203-207-Data-Structures-Algorithms/, CSE-205-Digital-Logic-Design/,
            CSE-211-Theory-of-Computation/, CSE-215-Database/, CSE-218-Numerical-Methods/
  level-3/  CSE-309-Compiler/, CSE-311-Data-Communication/, CSE-313-Operating-System/,
            CSE-317-Artificial-Intelligence/, CSE-321-Computer-Networks/,
            CSE-325-Information-System-Design/
  level-4/  CSE-405-Computer-Security/, CSE-409-Computer-Graphics/,
            CSE-461-Algorithm-Engineering-Reference-Books/

Sessional/
  level-1/  CSE-102-Structured-Programming-Sessional/, CSE-108-OOP-JavaFlix/
  level-2/  CSE-204-208-DSA-Sessional/, CSE-216-Database-Sessional/
  level-3/  CSE-307-308-Software-Engineering/, CSE-310-Compiler-Sessional/,
            CSE-314-Operating-System-Sessional/, CSE-318-Artificial-Intelligence-Sessional/,
            CSE-322-Computer-Networks-Sessional/
  level-4/  CSE-406-Computer-Security-Sessional/, CSE-410-Computer-Graphics-Sessional/,
            CSE-462-Algorithm-Engineering-Project/, CSE-472-Machine-Learning-Project/

resources/
  Books/                  General reference textbooks
  ConcreteMathematics/    Supplementary math course material
```

Courses with no distinct sessional (e.g. CSE 103, 211, 218, 311, 325) only exist under `Theory/`.
`CSE-203-207-Data-Structures-Algorithms` and `CSE-204-208-DSA-Sessional` combine both DSA I and
DSA II (203/204 and 207/208), since the source material didn't cleanly separate them by course
number — split instead by whether the content was lecture slides/reference vs. personal
offline/online assignment work.

## Repo mapping (source of full history)

| Folder here | Original repo |
|---|---|
| `Sessional/level-1/CSE-108-OOP-JavaFlix` | [CSE_108_OOP_Project_JavaFlix](https://github.com/Tusherbhomik/CSE_108_OOP_Project_JavaFlix) |
| `Theory/level-2/CSE-203-207-Data-Structures-Algorithms` (bulk) + `Sessional/level-2/CSE-204-208-DSA-Sessional` (bulk) | [DSA_SLIDES](https://github.com/Tusherbhomik/DSA_SLIDES) |
| `Sessional/level-3/CSE-307-308-Software-Engineering` | [CSE_308-SWE-Sessional](https://github.com/Tusherbhomik/CSE_308-SWE-Sessional) |
| `Sessional/level-3/CSE-310-Compiler-Sessional` | [CSE-310-Compiler-Design](https://github.com/Tusherbhomik/CSE-310-Compiler-Design) |
| `Sessional/level-3/CSE-314-Operating-System-Sessional` | [CSE-314-Operating-System](https://github.com/Tusherbhomik/CSE-314-Operating-System) |
| `Sessional/level-3/CSE-318-Artificial-Intelligence-Sessional` | [CSE-318-Artificial-Engineering-](https://github.com/Tusherbhomik/CSE-318-Artificial-Engineering-) |
| `Sessional/level-3/CSE-322-Computer-Networks-Sessional` | [CSE_322-Networking](https://github.com/Tusherbhomik/CSE_322-Networking) |
| `Sessional/level-4/CSE-406-Computer-Security-Sessional` | [Computer-Security-Sessional-406](https://github.com/Tusherbhomik/Computer-Security-Sessional-406) |
| `Sessional/level-4/CSE-410-Computer-Graphics-Sessional` | [Computer-Graphics-Sessional-410](https://github.com/Tusherbhomik/Computer-Graphics-Sessional-410) |
| `Sessional/level-4/CSE-462-Algorithm-Engineering-Project` | Merged from [Algorithm-Sessional-Project--461](https://github.com/Tusherbhomik/Algorithm-Sessional-Project--461) (base) + 2 unique files from [Algorithm-Sessional-Project-461](https://github.com/Tusherbhomik/Algorithm-Sessional-Project-461) |
| `Theory/level-4/CSE-461-Algorithm-Engineering-Reference-Books` | [CSE462-Algorithm-Engineering-Sessional](https://github.com/Tusherbhomik/CSE462-Algorithm-Engineering-Sessional) |
| `Sessional/level-4/CSE-472-Machine-Learning-Project` | [ML-Project-472](https://github.com/Tusherbhomik/ML-Project-472) |
| `resources/ConcreteMathematics` | [ConcreteMathematics](https://github.com/Tusherbhomik/ConcreteMathematics) |
| `resources/Books`, and every other `Theory/` folder plus the `Sessional/*-online-materials`-derived content | [BUET_CSE_ACADEMICS](https://github.com/Tusherbhomik/BUET_CSE_ACADEMICS) (still kept as a separate repo too) |

## Deduplication notes

- **`Algorithm-Sessional-Project-461` vs `--461`**: these were two copies of the same sessional
  project (421 of 423 files identical by content). Merged into `project-mkp/` using `--461` as the
  base (more commits) plus the 2 files only present in the single-dash copy
  (`Presentation/Omega_Group_9.pdf`, `report/Project_Report.pdf`).
- **Within-repo duplicate files** (same file saved in two places in the same original repo, e.g.
  loose working files also copied into a submission subfolder) were deduplicated, keeping the
  shallowest path. Affected `Computer-Graphics-Sessional-410` (Offline 3 Ray Tracing) and
  `DSA_SLIDES` (BalancedBST).
- **Build artifacts / IDE output** (`.class`, `.exe`, `.o`, `bin/`, `build/`, `.gradle/`, `.idea/`)
  were dropped as regenerable, not source content.
- Roughly 130 MB of redundant/regenerable data was removed in total; nothing unique was lost — all
  original repos remain intact on GitHub as the historical record.
- **`BUET_CSE_ACADEMICS` merge**: two differently-named "CSE 405 (Computer Security)" folders in
  that repo (mostly non-overlapping content, only 3 of ~60 files identical) were combined into one
  folder. Content already covered by an earlier merge (e.g. a duplicate copy of Sipser's
  *Introduction to the Theory of Computation* also present under `Theory/level-2/CSE-203-207-...`)
  was skipped by content hash rather than copied again. Every file was verified by content hash to
  be either present in the archive or a legitimate duplicate/build-artifact — none were silently
  lost.
- **Theory/Sessional split**: the archive was originally organized purely by course level; it was
  then reorganized into the `Theory/` vs `Sessional/` split described above. This was a pure move
  of existing files (no content changes). One file (`2005046.txt`, a small SQL practice file) was
  transiently lost to a directory-name collision during the move and was restored from the prior
  git commit; the whole move was then re-verified with `git status --find-renames` and a full
  content-hash reconciliation to confirm zero data loss.

## Course reference

See [`BUET_CSE_COURSES.md`](./BUET_CSE_COURSES.md) for the full BUET CSE curriculum course list,
grouped by level and term (course codes, titles, credit hours).

## Visibility

This repo is **private** — several folders contain copyrighted textbooks/solution manuals
(*Concrete Mathematics*, Sheldon Ross, *Encyclopedia of Algorithms*, CORMEN solutions) and private
coursework, matching the visibility of their original source repos.
