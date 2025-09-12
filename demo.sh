#!/bin/bash

# Demo script for Lynx Shell
echo "=== Lynx Shell Demo ==="
echo

# Test basic commands
echo "Testing basic commands:"
echo -e "pwd\nls -la\necho 'Hello from Lynx!'\ndate\nexit" | ./build/lynx

echo
echo "=== Demo Complete ==="
