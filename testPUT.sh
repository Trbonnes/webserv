mkdir -p curl_output
for i in $(seq 1 20)
do
    for j in $(seq 1 5 )
    do
        curl localhost:8080/put_test/lol --upload-file put_test/file_should_exist_after > curl_output/curl_output_$i 2>&1
    done &
done
wait