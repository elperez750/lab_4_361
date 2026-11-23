#!/usr/bin/env python3
"""
Simple tests for Lab 4.   Usage:  python3 simple_tests.py path/to/your/program

Use "a OR b" to accept either wording.
Each test = a list of program runs (each run = what you'd type, then Exit),
and words the LAST run's output must contain (HAS) or must not contain (HASNT).
Running several times in a row also tests "exit and restart" persistence.
"""
import shutil, struct, subprocess, sys, tempfile, time
from pathlib import Path

EXE = Path(sys.argv[1]).resolve()
DB = "../students.dat"           # how your program opens the file (from its working folder)

SAMPLE = [(101, "Jordan Lee", 21, 88, 1), (102, "Taylor Morgan Smith", 22, 95, 1),
          (103, "Alex Rivera", 24, 90, 1), (104, "Morgan Chen", 20, 76, 1),
          (105, "Casey Patel", 23, 82, 0)]


def run_program(work, lines):
    """Runs the program once. Returns (None, None) if it hangs or keeps printing forever."""
    out_path = work / "out.txt"
    with open(out_path, "w") as out_file:
        p = subprocess.Popen([str(EXE)], cwd=work, stdin=subprocess.PIPE, stdout=out_file,
                             stderr=subprocess.STDOUT, text=True)
        try:
            p.stdin.write("\n".join(lines) + "\n")
            p.stdin.close()
        except OSError:
            pass
        start = time.time()
        while p.poll() is None:
            if time.time() - start > 5 or out_path.stat().st_size > 2_000_000:
                p.kill()
                p.wait()
                return None, None
            time.sleep(0.05)
    return out_path.read_text(errors="replace"), p.returncode


def one_test(name, runs, has=(), hasnt=(), db="sample", eof=False):
    tmp = Path(tempfile.mkdtemp())
    work = tmp / "work"
    work.mkdir()
    if db == "sample":
        (tmp / "students.dat").write_bytes(b"".join(
            struct.pack("<i50s2xiii", i, n.encode(), a, s, act) for i, n, a, s, act in SAMPLE))
    elif db == "empty":
        (tmp / "students.dat").write_bytes(b"")
    out = rc = None
    for lines in runs:
        out, rc = run_program(work, lines if eof else lines + ["0"])
        if out is None:
            break
    shutil.rmtree(tmp, ignore_errors=True)

    expected = "; ".join([f"shows '{w}'" for w in has] + [f"does not show '{w}'" for w in hasnt])
    if out is None:
        actual = "program hung"
    elif rc != 0:
        actual = f"exit status {rc}"
    else:
        low = out.lower()
        bad = [f"missing '{w}'" for w in has if not any(a.lower() in low for a in w.split(" OR "))] + \
              [f"unexpected '{w}'" for w in hasnt if w.lower() in low]
        actual = "; ".join(bad) or "as expected"
    result = "Pass" if actual == "as expected" else "Fail"
    print(f"| {name} | {expected or 'program ends cleanly'} | {actual} | {result} |")


print("| Test | Expected | Actual | Result |\n|---|---|---|---|")
DISP, A33, N = ["2"], "33", "66"
def add(i, n, a, s): return ["1", str(i), n, str(a), str(s)]
def srch(i): return ["3", str(i)]
def upd(i, n, a, s): return ["4", str(i), n, str(a), str(s)]
def dele(i, c): return ["5", str(i), str(c)]
def bad_input(*bad):      # bad values typed at the age prompt, then a good age/score
    return ["1", "110", "Input Test", *bad, "33", "66"]

one_test("Initial display", [DISP], ["Jordan Lee", "Taylor Morgan Smith", "Alex Rivera", "Morgan Chen"], ["Casey Patel"])
one_test("Add student (and restart)", [add(106, "Jamie Park", 19, 84), DISP], ["Jamie Park"])
one_test("Duplicate ID", [add(101, "Impostor", 30, 70), DISP], ["Jordan Lee"], ["Impostor"])
one_test("Successful search", [srch(103)], ["Alex Rivera"])
one_test("Missing student", [srch(999)], ["not found OR does not exist"])
one_test("Deleted student", [srch(105)], ["not found OR does not exist"], ["Casey Patel"])
one_test("Update student (and restart)", [upd(101, "Jordan A. Lee", 23, 93), srch(101)], ["Jordan A. Lee", "93"])
one_test("Preserve others", [upd(101, "Jordan A. Lee", 23, 93), DISP], ["Taylor Morgan Smith"])
one_test("Missing update", [["4", "999"]], ["not found OR does not exist"])
one_test("Cancel deletion", [dele(102, 0), DISP], ["Taylor Morgan Smith"])
one_test("Confirm deletion (and restart)", [dele(102, 1), DISP], ["Jordan Lee"], ["Taylor Morgan Smith"])
one_test("Repeated deletion", [dele(102, 1), ["5", "102"]], ["not found OR does not exist"])
one_test("Reuse deleted ID", [add(105, "Reused Five", 30, 70), srch(105)], ["Reused Five"])
one_test("Invalid menu", [["9"]], ["invalid"])
one_test("Invalid confirmation", [["5", "102", "7", "1"], DISP], ["Jordan Lee"], ["Taylor Morgan Smith"])
one_test("Nonnumeric input", [bad_input("abc"), srch(110)], ["Input Test", N])
one_test("Decimal input", [bad_input("21.5"), srch(110)], ["Input Test", N])
one_test("Trailing text", [bad_input("21abc"), srch(110)], ["Input Test", N])
one_test("Invalid ID", [["1", "0", "-5", "110", "Input Test", "33", "66"], srch(110)], ["Input Test", N])
one_test("Large integer", [bad_input("99999999999999999999"), srch(110)], ["Input Test", N])
one_test("Age boundaries", [add(121, "Young Age", 1, 50), add(122, "Old Age", 120, 50), DISP], ["Young Age", "Old Age"])
one_test("Invalid ages", [bad_input("0", "121"), srch(110)], ["Input Test", N])
one_test("Score boundaries", [add(121, "Low Score", 30, 0), add(122, "High Score", 30, 100), DISP], ["Low Score", "High Score"])
one_test("Invalid scores", [["1", "110", "Input Test", "33", "-1", "101", "66"], srch(110)], ["Input Test", N])
one_test("Blank name", [["1", "110", "   ", "Input Test", "33", "66"], srch(110)], ["Input Test", N])
one_test("Maximum name (49 chars)", [add(123, "A" * 49, 30, 70), DISP], ["A" * 25])
one_test("Overlong name (50 chars)", [["1", "110", "A" * 50, "Input Test", "33", "66"], DISP], ["Input Test"], ["A" * 25])
one_test("Database creation", [add(1, "First Person", 20, 50), DISP], ["First Person"], db="missing")
one_test("End of input", [["1", "106"]], eof=True)