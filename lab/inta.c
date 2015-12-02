#define MAX 5

void change(int ns[]) {
  ns[0] = 111;
  ns[1] = 222;
  ns[2] = 333;
}

int main() {
  int i, nums[MAX];
  memset(nums, 0, sizeof(nums));
  change(nums);
  for (i = 0; i < MAX; i++)
    printf("nums[%d] = %d\n", i, nums[i]);
  return 0;
}
