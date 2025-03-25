diff --git a/sshconnect2.c b/sshconnect2.c
index 347e348c..f64aae66 100644
--- a/sshconnect2.c
+++ b/sshconnect2.c
@@ -1,4 +1,4 @@
-/* $OpenBSD: sshconnect2.c,v 1.325 2020/08/27 01:06:18 djm Exp $ */
+/* $OpenBSD: sshconnect2.c,v 1.326 2020/09/18 05:23:03 djm Exp $ */
 /*
  * Copyright (c) 2000 Markus Friedl.  All rights reserved.
  * Copyright (c) 2008 Damien Miller.  All rights reserved.
@@ -102,12 +102,25 @@ verify_host_key_callback(struct sshkey *hostkey, struct ssh *ssh)
 	return 0;
 }
 
+/* Returns the first item from a comma-separated algorithm list */
+static char *
+first_alg(const char *algs)
+{
+	char *ret, *cp;
+
+	ret = xstrdup(algs);
+	if ((cp = strchr(ret, ',')) != NULL)
+		*cp = '\0';
+	return ret;
+}0
+
 static char *
 order_hostkeyalgs(char *host, struct sockaddr *hostaddr, u_short port)
 {
-	char *oavail, *avail, *first, *last, *alg, *hostname, *ret;
+	char *oavail = NULL, *avail = NULL, *first = NULL, *last = NULL;
+	char *alg = NULL, *hostname = NULL, *ret = NULL, *best = NULL;
 	size_t maxlen;
-	struct hostkeys *hostkeys;
+	struct hostkeys *hostkeys = NULL;
 	int ktype;
 	u_int i;
 
@@ -119,6 +132,26 @@ order_hostkeyalgs(char *host, struct sockaddr *hostaddr, u_short port)
 	for (i = 0; i < options.num_system_hostfiles; i++)
 		load_hostkeys(hostkeys, hostname, options.system_hostfiles[i]);
 
+	/*
+	 * If a plain public key exists that matches the type of the best
+	 * preference HostkeyAlgorithms, then use the whole list as is.
+	 * Note that we ignore whether the best preference algorithm is a
+	 * certificate type, as sshconnect.c will downgrade certs to
+	 * plain keys if necessary.
+	 */
+	best = first_alg(options.hostkeyalgorithms);
+	if (lookup_key_in_hostkeys_by_type(hostkeys,
+	    sshkey_type_plain(sshkey_type_from_name(best)), NULL)) {
+		debug3("%s: have matching best-preference key type %s, "
+		    "using HostkeyAlgorithms verbatim", __func__, best);
+		ret = xstrdup(options.hostkeyalgorithms);
+		goto out;
+	}
+
+	/*
+	 * Otherwise, prefer the host key algorithms that match known keys
+	 * while keeping the ordering of HostkeyAlgorithms as much as possible.
+	 */
 	oavail = avail = xstrdup(options.hostkeyalgorithms);
 	maxlen = strlen(avail) + 1;
 	first = xmalloc(maxlen);
@@ -159,6 +192,8 @@ order_hostkeyalgs(char *host, struct sockaddr *hostaddr, u_short port)
 	if (*first != '\0')
 		debug3("%s: prefer hostkeyalgs: %s", __func__, first);
 
+ out:
+	free(best);
 	free(first);
 	free(last);
 	free(hostname);