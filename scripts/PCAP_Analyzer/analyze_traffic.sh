#!/bin/bash

# Terminal Colors #
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

function check_pcap()
{
    if (( $# < 1 )); then
        echo "No input files found"
        exit 1
    fi

    if [[  ${1##*.} != "$2" ]]; then
        echo "Please use .pcap file extension"
        exit 1
    fi

    if [ ! -f "$1" ]; then
        echo "PCAP file does not exist"
        exit 1
    fi
}

function analyze_traffic()
{

    tshark -r "$1" > "$2"

    echo "----- Network Traffic Analysis Report -----"

    TOTAL_PACKETS=$(get_total_packets "$2")
    PROTOCOLS=$(get_protocols "$2")
    SRC_IPS=$(get_src_ips "$2")
    DST_IPS=$(get_dest_ips "$2")
    
    printf "\n1. Total Packets: ${GREEN}%s\n${NC}" "$TOTAL_PACKETS"

    printf "\n2. Protocols:\n%s\n" "$PROTOCOLS"

    printf "\n3. Top source IP addresses:\n%s\n" "$SRC_IPS"

    printf "\n4. Top destination IP addresses:\n%s\n\n" "$DST_IPS"


    echo "-------------- End of Report --------------"
}

function get_total_packets()
{
    local filename="$1"
    local LINES=0
    while IFS= read -r line; do
        ((LINES++))
    done < "$filename"

    echo $LINES

    return 0
}

function get_protocols()
{
    local filename="$1"
    declare -A protocol_count

    while read -r _ _ _ _ _ proto _; do
        ((protocol_count["$proto"]++))
    done < "$filename"

    for proto in "${!protocol_count[@]}"; do
        printf "\t- ${RED}%s${NC}: %d packets\n" "$proto" "${protocol_count[$proto]}"
    done

    return 0
}

function get_src_ips()
{
    local filename="$1"
    declare -A protocol_count

    while read -r _ _ proto _ _ _ _; do
        ((protocol_count["$proto"]++))
    done < "$filename"

    for proto in "${!protocol_count[@]}"; do
        echo "${protocol_count[$proto]} $proto"
    done | sort -nr | while read -r count proto; do
        printf "\t- ${RED}%s${NC}: %d packets\n" "$proto" "$count"
    done

    return 0
}

function get_dest_ips()
{
    local filename="$1"
    declare -A protocol_count

    while read -r _ _ _ _ proto _ _; do
        ((protocol_count["$proto"]++))
    done < "$filename"

    for proto in "${!protocol_count[@]}"; do
        echo "${protocol_count[$proto]} $proto"
    done | sort -nr | while read -r count proto; do
        printf "\t- ${RED}%s${NC}: %d packets\n" "$proto" "$count"
    done

    return 0
}

function main()
{

    declare PCAP_FILE
    declare EXT="pcap"
    STATS_FILE="traffic.txt"

    check_pcap "$1" "$EXT"

    PCAP_FILE="$1"

    printf "\nTarget File: ${YELLOW}%s\n\n${NC}" "$PCAP_FILE"

    analyze_traffic "$PCAP_FILE" "$STATS_FILE"
}

main "$1"
