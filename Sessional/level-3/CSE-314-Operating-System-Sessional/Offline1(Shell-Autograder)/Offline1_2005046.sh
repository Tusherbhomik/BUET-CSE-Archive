#!/usr/bin/bash

#used absolute path
# issues_directory="/mnt/e/32/home/output/issues"
# checked_directory="/mnt/e/32/home/output/checked"
#output directory must be processsed first 
# py alloewd not python 
#./2005046.sh -i input_file.txt run command

#
rm -rf "/mnt/e/32/home/output"
input_file=$2

count_lines() {
    local file_path=$1
    local line_count
    line_count=$(wc -l <"$file_path")
    echo "$line_count"
}

validate_input() {
    local file_path=$1
    local line_count=$(count_lines "$file_path")
    #line count check
    if [[ $line_count -ne 11 ]]; then
        echo "Error: File is not valid."
        return 1
    fi
    # first line validation
    #_______________________________________________________________________
    local first_line
    first_line=$(sed -n '1p' "$file_path" | tr -d '\r' | xargs) # Remove
    if [[ "$first_line" != "true" && "$first_line" != "false" ]]; then
        echo "Error: The first line must contain only 'true' or 'false'."
        return 1
    fi
    #_______________________________________________________________________
    #2nd line validation
    local allowed_formats=("zip" "rar" "tar")
    local second_line=$(sed -n '2p' "$file_path" | tr -d '\r')
    local IFS=' '
    local format_valid=1

    read -r -a formats <<<"$second_line"
    for format in "${formats[@]}"; do
        if [[ ! " ${allowed_formats[@]} " =~ " ${format} " ]]; then
            format_valid=0
            break
        fi
    done
    if [[ "$format_valid" -eq 0 ]]; then
        echo "Invalid format in the second line. Allowed formats are: ${allowed_formats[*]}"
        return 1
    fi
    #_____________________________________________________________________
    #3rd line
    local allowed_languages=("c" "cpp" "py" "sh")
    local third_line=$(sed -n '3p' "$file_path" | tr -d '\r' | xargs)
    local IFS=' '
    local language_valid=1

    read -r -a language_formats <<<"$third_line"
    for format in "${language_formats[@]}"; do
        if [[ ! " ${allowed_languages[@]} " =~ " ${format} " ]]; then
            language_valid=0
            break
        fi
    done
    if [[ "$language_valid" -eq 0 ]]; then
        echo "Error: Invalid format in the third line. Allowed formats are: ${allowed_languages[*]}."
        return 1
    fi
    #_____________________________________________________________________
    #4th line
    local fourth_line=$(sed -n '4p' "$file_path" | tr -d '\r' | xargs)
    if ! [[ "$fourth_line" =~ ^[0-9]+$ ]]; then
        echo "Error: The fourth line must contain a valid integer."
        return 1
    fi
    #_____________________________________________________________________
    #5th line
    local fifth_line=$(sed -n '5p' "$file_path" | tr -d '\r' | xargs)
    if ! [[ "$fifth_line" =~ ^[0-9]+$ ]]; then
        echo "Error: The fifth line must contain a valid integer."
        return 1
    fi
    #_____________________________________________________________________

    #6th line
    local sixth_line=$(sed -n '6p' "$file_path" | tr -d '\r' | xargs)
    if [[ ! -d "$sixth_line" ]]; then
        echo "Error: The directory specified in the sixth line does not exist."
        return 1
    fi
    #_____________________________________________________________________

    #7th line
    local seventh_line=$(sed -n '7p' "$file_path" | tr -d '\r' | xargs)
    local IFS=' '
    read -r start_id end_id <<<"$seventh_line"

    if ! [[ "$start_id" =~ ^[0-9]+$ && "$end_id" =~ ^[0-9]+$ ]]; then
        echo "Error: The seventh line must contain valid integers."
        return 1
    fi

    if [[ "$end_id" -le "$start_id" ]]; then
        echo "Error: The end ID must be greater than the start ID."
        return 1
    fi
    #_____________________________________________________________________
    #8th line
    local eighth_line=$(sed -n '8p' "$file_path" | tr -d '\r' | xargs)
    if [[ ! -f "$eighth_line" ]]; then
        echo "Error: The file specified in the eighth line does not exist."
        return 1
    fi
    #_____________________________________________________________________
    #9th Line
    local ninth_line=$(sed -n '9p' "$file_path" | tr -d '\r' | xargs)
    if ! [[ "$ninth_line" =~ ^[0-9]+$ ]]; then
        echo "Error: The ninth line must contain a valid integer."
        return 1
    fi

    #_____________________________________________________________________
    #10th line
    local tenth_line=$(sed -n '10p' "$file_path" | tr -d '\r' | xargs)
    if [[ ! -f "$tenth_line" ]]; then
        echo "Error: The file specified in the Tenth line does not exist."
        return 1
    fi
    #_____________________________________________________________________
    #11th line
    local eleventh_line=$(sed -n '9p' "$file_path" | tr -d '\r' | xargs)
    if ! [[ "$eleventh_line" =~ ^[0-9]+$ ]]; then
        echo "Error: The eleventh line must contain a valid integer."
        return 1
    fi
    #_____________________________________________________________________
    echo "File is valid."
    return 0
}

count_missing_lines() {
    local expected_output_file="$1"
    local student_output_file="$2"

    if [[ -f "$expected_output_file" ]]; then
        mapfile -t expected_lines <"$expected_output_file"
        mapfile -t student_lines <"$student_output_file"
        local missing_count=0
        for expected_line in "${expected_lines[@]}"; do
            if ! printf '%s\n' "${student_lines[@]}" | grep -Fxq "$expected_line"; then
                ((missing_count++))
            fi
        done
        echo "$missing_count"
    fi
}

if validate_input "$input_file"; then
    echo "Proceeding with the evaluation..."
    read -r archieve < <(sed -n '1p' "$input_file" | tr -d '\r')
    read -r allowed_archives < <(sed -n '2p' "$input_file" | tr -d '\r')
    read -r allowed_languages < <(sed -n '3p' "$input_file" | tr -d '\r')
    read -r total_marks < <(sed -n '4p' "$input_file" | tr -d '\r')
    read -r unmatched_penalty < <(sed -n '5p' "$input_file" | tr -d '\r')
    read -r working_directory < <(sed -n '6p' "$input_file" | tr -d '\r')
    read -r student_id_range < <(sed -n '7p' "$input_file" | tr -d '\r')
    read -r expected_output_file < <(sed -n '8p' "$input_file" | tr -d '\r')
    read -r submission_penalty < <(sed -n '9p' "$input_file" | tr -d '\r')
    read -r plagiarism_file < <(sed -n '10p' "$input_file" | tr -d '\r')
    read -r plagiarism_penalty < <(sed -n '11p' "$input_file" | tr -d '\r')

    # Input manipulate kortesi ekhane
    allowed_languages_array=($allowed_languages)
    allowed_archives_array=($allowed_archives)
    allowed_archives_regex=$(printf "|%s" "${allowed_archives_array[@]}")
    allowed_archives_regex=${allowed_archives_regex:1}
    working_directory=$(echo "$working_directory" | tr -d '\r' | xargs)
    IFS=' ' read -r start_id end_id <<<"$student_id_range"

    # Create and/or clear the issues and checked directories
    issues_directory="/mnt/e/32/home/output/issues"
    checked_directory="/mnt/e/32/home/output/checked"

    mkdir -p "$issues_directory" "$checked_directory"
    rm -rf "$issues_directory"/* "$checked_directory"/*

    processed_ids=()
    marks_file="marks.csv"
    echo "id,marks,marks_deducted,total_marks,remarks" >"$marks_file"

    # Directories for submissions and issued files
    submissions_directory="/mnt/e/32/home/output/submissions"
    mkdir -p "$submissions_directory"

    for file in "$working_directory"/*; do
        if [[ -d "$file" ]]; then
            # Handle directory submissions
            echo "Submission is a directory. Deducting penalty."
            student_id=$(basename "$file")
            student_marks=$total_marks
            marks_cut=$submission_penalty
            total_marks=$((student_marks - marks_cut))
            # student_marks=$((student_marks - submission_penalty))
            cp -r "$file" "$submissions_directory/"
            echo "$student_id,"0","$submission_penalty","0",issue case #1" >>"$marks_file"

        elif [[ -f "$file" ]]; then
            # Handle file submissions
            filename=$(basename "$file")
            base_name="${filename%.*}"

            if [[ "$archieve" == "false" ]]; then
                # For non-archive files when 'archive' is false
                echo "Handling non-archive file."
                mkdir -p "$submissions_directory/$base_name"
                cp "$file" "$submissions_directory/$base_name/"
                echo "$filename moved to '$submissions_directory/$base_name/'"

            else
                # Handle archive files when 'archive' is true
                echo "Handling archive file."
                file_extension="${filename##*.}"

                if [[ " ${allowed_archives_array[@]} " =~ " ${file_extension} " ]]; then
                    case "$file_extension" in
                    zip)
                        unzip "$file" -d "$submissions_directory" >/dev/null 2>&1
                        ;;
                    rar)
                        unrar x "$file" "$submissions_directory" >/dev/null 2>&1
                        ;;
                    tar)
                        tar -xf "$file" -C "$submissions_directory" >/dev/null 2>&1
                        ;;

                    esac
                else
                    echo "$filename: not a valid archive format. Allowed formats are: ${allowed_archives_array[*]}"
                    student_marks=$((student_marks - submission_penalty))
                    echo "$student_id,"0","$submission_penalty","0",issue case #2" >>"$marks_file"
                    continue
                fi
            fi

        else
            # Handle files that are neither directories nor regular files
            echo "$file: Not a valid submission format. Ignoring."
        fi
    done
    # Process student submissions
    for student_dir in "$submissions_directory"/*; do
        if [[ -d "$student_dir" ]]; then
            student_id=$(basename "$student_dir")
            student_marks=$total_marks
            processed_ids+=("$student_id")

            if [[ ! "$student_id" =~ ^[0-9]+$ ]] || ((student_id < start_id || student_id > end_id)); then
                echo "Folder name '$student_id' does not match a valid student ID. Logging the issue but proceeding with the evaluation."
                echo "$student_id: Folder name mismatch with student ID" >>"/mnt/e/32/home/output/issued/issues.log"
                mv "$student_dir" "$issues_directory/"
                echo "$student_id,"0",$submission_penalty,"0",issue case-4" >>"$marks_file"
                continue
            fi

            # Evaluate each file in the student's directory
            issue_flag=0
            for file in "$student_dir"/*; do
                if [[ -f "$file" ]]; then
                    filename=$(basename "$file")
                    file_extension="${filename##*.}"
                    base_filename="${filename%.*}"
                    output_file="${student_dir}/${base_filename}_output.txt"

                    # Check if the file extension is in the allowed languages list
                    if [[ " ${allowed_languages_array[@]} " =~ " ${file_extension} " ]]; then
                        case "$file_extension" in
                        c)
                            gcc "$file" -o "${file%.c}" && "${file%.c}" >"$output_file" 2>&1
                            ;;
                        cpp)
                            g++ "$file" -o "${file%.cpp}" && "${file%.cpp}" >"$output_file" 2>&1
                            ;;
                        py)
                            python3 "$file" >"$output_file" 2>&1
                            ;;
                        sh)
                            bash "$file" >"$output_file" 2>&1
                            ;;
                        *)
                            echo "File $file has an unsupported extension."
                            ;;
                        esac

                        missing_count=$(count_missing_lines "$expected_output_file" "$output_file")
                        marks_deducted=$((missing_count * unmatched_penalty))
                        student_marks=$((student_marks - marks_deducted))

                        line=$(grep "^$student_id," "$marks_file")
                        IFS=',' read -r extracted_student_id extracted_student_marks extracted_marks_deducted extracted_total_marks extracted_remarks <<<"$line"
                        extracted_marks_deducted=${extracted_marks_deducted:-0}
                        echo "$student_id,$student_marks,$extracted_marks_deducted,$((student_marks - extracted_marks_deducted)),$extracted_remarks" >>"$marks_file"

                    else
                        echo "File $file with extension $file_extension is not in the allowed languages list."
                        echo "$filename: Programming language $file_extension is not allowed. Deducting submission penalty and skipping evaluation."

                        student_marks=$((student_marks - submission_penalty))
                        echo "$student_id,"0",$submission_penalty,"-$submission_penalty",issue case -3" >>"$marks_file"
                        issue_flag=1
                        break
                    fi
                fi
            done

            # Move directories based on evaluation outcome
            if [[ $issue_flag -eq 1 ]]; then
                mv "$student_dir" "$issues_directory/"
            else
                mv "$student_dir" "$checked_directory/"
            fi
        fi

    done

    # Add missing submissions to the marks file
    for id in $(seq "$start_id" "$end_id"); do
        if [[ ! " ${processed_ids[@]} " =~ " ${id} " ]]; then
            echo "$id,0,0,0,missing submission" >>"$marks_file"
        fi
    done

    # Apply plagiarism penalty
    if [[ -f "$plagiarism_file" ]]; then
        mapfile -t plagiarized_students <"$plagiarism_file"
        for plagiarized_student_id in "${plagiarized_students[@]}"; do
            plagiarized_student_id=$(echo "$plagiarized_student_id" | tr -d '\r')

            if [[ -d "$submissions_directory/$plagiarized_student_id" ]]; then
                mv "$submissions_directory/$plagiarized_student_id" "/mnt/e/32/home/output/issued/"
            fi

            if grep -q "^$plagiarized_student_id," "$marks_file"; then
                line=$(grep "^$plagiarized_student_id," "$marks_file")
                IFS=',' read -r extracted_student_id extracted_student_marks extracted_marks_deducted extracted_total_marks <<<"$line"
                sed -i "s/^$plagiarized_student_id,.*/$plagiarized_student_id,$extracted_student_marks,$extracted_marks_deducted,"-$plagiarism_penalty",Plagiarism detected/" "$marks_file"
            fi
        done
    else
        echo "Plagiarism file not found. Skipping plagiarism check."
    fi

else
    echo "Input validation failed. Exiting."
    exit 1 # Exit the script if validation fails
fi
