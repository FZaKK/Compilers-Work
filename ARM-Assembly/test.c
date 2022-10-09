int main() {
	int a[5];
	int ans;
	int i;
	i = 0;
	ans = getint();
	//cin >> ans;
	while (i < 5) {
		a[i] = i + 1;
		ans = ans | a[i]; //todo 'or' operation
		i++;
	}
	//cout << ans;
	putint(ans);
	return 0;
}
