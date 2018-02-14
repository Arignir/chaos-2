if (( $# != 2 ))
then
    echo "usage: ./${0} in_dir out_file"
fi

in_dir=${1}
out_file=${2}

shopt -s nullglob

>${out_file}

add_int() {
    local x=${1}
    local out;
    printf -v out '\\x%02x\\x%02x\\x%02x\\x%02x' $((x & 255)) $((x >> 8 & 255)) $((x >> 16 & 255)) $((x >> 24 & 255))
    printf ${out} >> ${out_file}
}

add_file() {
    echo "adding ${1} to dumbfsfile system"

    local file_name=${1}
    local file_path=${in_dir}/${file_name}
    local name_size=${#file_name}
    local file_size=$(stat -c%s ${file_path})
    local entry_size=$((file_size + name_size))
    add_int ${entry_size}
    add_int ${file_size}
    echo -nE "${file_name}"
    echo -nE "${file_name}" >> ${out_file}
    printf '\x00' >> ${out_file}
    cat ${file_path} >> ${out_file}
}

files=(${in_dir}/*)

echo ${files}
echo "found ${#files[@]} files"

add_int ${#files[@]}
for file in ${files[@]}
do
    add_file ${file##*/};
done
