# $1 should be 1(multi dimension) or 0(one dimension)

if [ -f "test-common/integrationtest/setup.sh" ]
then
    sh test-common/integrationtest/runall.sh $1
fi